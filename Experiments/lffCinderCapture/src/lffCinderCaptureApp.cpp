
#include "cinder/app/AppBasic.h"
#include "cinder/Serial.h"
#include "cinder/params/Params.h"
#include "cinder/Text.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Camera.h"
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Texture.h"
#include "boost/format.hpp"

#include <unistd.h>
#include <time.h>

#include <dispatch/dispatch.h>
#include <ApplicationServices/ApplicationServices.h>

#include "BracketBuffer.h"
#include "Resources.h"


#pragma mark Defines

#define CAMERA_FRAMES_COUNT 30
#define CAMERA_STATUS_TEXT_SIZE 512

#define SERIAL_BUFSIZE 1024
#define SERIAL_READ_INTERVAL 0.25

static const bool PREMULT = false;

#pragma mark Namespaces

using namespace ci;
using namespace ci::app;
using namespace std;

#pragma mark Types

// Ancillary Data
typedef struct {
    uint32_t acquisitionCount;
    uint32_t userValue;
    uint32_t exposureValue;
    uint32_t gainValue;
    uint32_t syncInLevels;
    uint32_t syncOutLevels;
    uint32_t counter1value;
    uint32_t counter2value;
    uint32_t timer1value;
    uint32_t timer2value;
    uint32_t sequenceCount;
    uint32_t chunkID;
    uint32_t chunkLength;
} pvFrameAncillaryData;

// Camera's data
typedef struct 
{
    unsigned long   UID;
    tPvHandle       Handle;
    tPvFrame        Frames[CAMERA_FRAMES_COUNT];
    tPvUint32       Counter;
    pthread_t       ThHandle;
	bool            Abort;
    unsigned long   FrameCompleted;
    unsigned long   FrameDropped;
    unsigned long   PacketReceived;
    unsigned long   PacketMissed;
    char            StatusText[CAMERA_STATUS_TEXT_SIZE];
    float           FrameRate;
} tCamera;

enum frameState {
    FRAME_STATE_QUEUED = 0,
    FRAME_STATE_PROCESSING = 1
};

frameState gFramestateQueued = FRAME_STATE_QUEUED;
frameState gFramestateProcessing = FRAME_STATE_PROCESSING;

#pragma mark Classes

class lffCinderCaptureApp : public AppBasic {
    
public:
    ~lffCinderCaptureApp();
    
    void prepareSettings( Settings *settings );
    
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    void processFrame( tPvFrame* pFrame );
    void renderBufferToFbo(BracketBuffer* buffer, gl::Fbo* fbo);
    void renderHdrTexture(gl::Texture *tex);
    
    vector <BracketBuffer*> bracketBuffers;
    vector <BracketBuffer*> averageBuffers;
    
    vector <gl::Fbo> hdrFbos;
    
    params::InterfaceGl	mParams;
    
    void keyDown( KeyEvent event );
    
    gl::GlslProg hdrDebayerShader;
    
    int justUpdatedBracket;
    int lastUpdatedBracket;
    int displayBracket;
    
    vector<gl::Texture>	textTextures;
    
    int cameraWidth;
    int cameraHeight;
    
    int cameraGain;
    int cameraNumberBrackets;
    int cameraBracketEv;
    unsigned long cameraBaseExposure;
    
    std::string sendSerialCommand(const std::string _command);
    
    void setCameraNumberBrackets(int _numberBrackets);
    void setCameraBracketEv(int _bracketEv);
    void setCameraBaseExposure(unsigned long _baseExposure);
    void printCameraParameters();
    
	Serial serial;
    
};

#pragma mark Globals

// global camera data
tCamera GCamera;
bool PVAPIinitialised;

// global app pointer
lffCinderCaptureApp * theApp;

#pragma mark Global Functions

//Thread start function. Displays camera stats.
void *ThreadFunc(void *pContext)
{
    tPvErr Err;
    
    //StatFramesCompleted increments when a queued frame returns with tPvFrame.Status = ePvErrSuccess
	//StatFramesDropped increments when a queued frame returns with tPvFrame.Status = ePvErrDataMissing
	//In a situation where a camera returns a frame, but there is no frame queued for it, THESE
	//STATS DO NOT INCREMENT. tPvFrame.FrameCount increments regardless of host queuing, and is a better measure
	//for what frame is being returned from the camera. See CameraStart, where we check this parameter,
	//for the case where a frame is returned from camera with no frame queued on host.
	while(!GCamera.Abort &&
          ((Err = PvAttrUint32Get(GCamera.Handle,"StatFramesCompleted",&GCamera.FrameCompleted)) == ePvErrSuccess) &&
          ((Err = PvAttrUint32Get(GCamera.Handle,"StatFramesDropped",&GCamera.FrameDropped)) == ePvErrSuccess) &&
		  ((Err = PvAttrUint32Get(GCamera.Handle,"StatPacketsMissed",&GCamera.PacketMissed)) == ePvErrSuccess) &&
		  ((Err = PvAttrUint32Get(GCamera.Handle,"StatPacketsReceived",&GCamera.PacketReceived)) == ePvErrSuccess) &&
          ((Err = PvAttrFloat32Get(GCamera.Handle,"StatFrameRate",&GCamera.FrameRate)) == ePvErrSuccess))
    {
        usleep(20*1000);
	}
    
    return 0;
}

// Create a thread to display camera stats. This could also be done in the frame callback.
// By spawning a new thread, we're able to display stats independent of frames returning. 
void SpawnThread()
{
    pthread_create(&GCamera.ThHandle,NULL,ThreadFunc,(void *)&GCamera);
}

// wait for the thread to be over
void WaitThread()
{
    pthread_join(GCamera.ThHandle,NULL);
}

// wait for camera to be plugged in
void WaitForCamera()
{
    printf("Waiting for a camera ");
    while((PvCameraCount() == 0) && !GCamera.Abort)
    {
        printf(".");
        usleep(250*1000);
    }
    printf("\n");
}

// Frame completed callback executes on seperate driver thread.
// One callback thread per camera. If a frame callback function has not 
// completed, and the next frame returns, the next frame's callback function is queued. 
// This situation is best avoided (camera running faster than host can process frames). 
// Spend as little time in this thread as possible and offload processing
// to other threads or save processing until later.
//
// Note: If a camera is unplugged, this callback will not get called until PvCaptureQueueClear.
// i.e. callback with pFrame->Status = ePvErrUnplugged doesn't happen -- so don't rely
// on this as a test for a missing camera. 
void FrameDoneCB(tPvFrame* pFrame)
{ 
	// if frame hasn't been cancelled, requeue frame
    if(pFrame->Status != ePvErrCancelled){
        
        if(pFrame->Status == ePvErrSuccess)
        {
            pFrame->Context[0] = &gFramestateProcessing;
            theApp->processFrame(pFrame);
        }
        else
        {
            if (pFrame->Status == ePvErrDataMissing)
                printf("Dropped packets. Possible improper network card settings:\nSee GigE Installation Guide.");
            else
                printf("Frame.Status error: %u\n",pFrame->Status);
        }
        pFrame->Context[0] = &gFramestateQueued;
        PvCaptureQueueFrame(GCamera.Handle,pFrame,FrameDoneCB);
    }
}

// open camera, allocate memory
// return value: true == success, false == fail
bool CameraSetup()
{
    tPvErr errCode;
	unsigned long FrameSize = 0;
    unsigned long AncillaryBufferSize = 0;
    bool failed = false;
    
	//open camera
	if ((errCode = PvCameraOpen(GCamera.UID,ePvAccessMaster,&(GCamera.Handle))) != ePvErrSuccess)
	{
		if (errCode == ePvErrAccessDenied)
			printf("PvCameraOpen returned ePvErrAccessDenied:\nCamera already open as Master, or camera wasn't properly closed and still waiting to HeartbeatTimeout.");
		else
			printf("PvCameraOpen err: %u\n", errCode);
		return false;
	}
    
	// Activate Chunk Mode
    if((errCode = PvAttrBooleanSet(GCamera.Handle,"ChunkModeActive", TRUE)) != ePvErrSuccess)
	{
		printf("CameraSetup: Failed to activate Chunkmode: %u\n", errCode);
		return false;
	}
    
	// Calculate frame buffer size
    if((errCode = PvAttrUint32Get(GCamera.Handle,"TotalBytesPerFrame",&FrameSize)) != ePvErrSuccess)
	{
		printf("CameraSetup: Get TotalBytesPerFrame err: %u\n", errCode);
		return false;
	}
    
    // Calculate ancillary buffer size
    if((errCode = PvAttrUint32Get(GCamera.Handle,"NonImagePayloadSize",&AncillaryBufferSize)) != ePvErrSuccess)
	{
		printf("CameraSetup: Get NonImagePayloadSize err: %u\n", errCode);
		return false;
	}
    
    // allocate the frame buffers
    for(int i=0;i<CAMERA_FRAMES_COUNT && !failed;i++)
    {
        int * frameNumber = new int(i);
        GCamera.Frames[i].Context[1] = frameNumber;
        
        GCamera.Frames[i].ImageBuffer = new char[FrameSize];
        if(GCamera.Frames[i].ImageBuffer)
        {
			GCamera.Frames[i].ImageBufferSize = FrameSize;
		}
        else
		{
			printf("CameraSetup: Failed to allocate Frame buffers.\n");
			failed = true;
		}
        GCamera.Frames[i].AncillaryBuffer = new pvFrameAncillaryData;
        if(GCamera.Frames[i].AncillaryBuffer)
        {
			GCamera.Frames[i].AncillaryBufferSize = AncillaryBufferSize;
		}
        else
		{
			printf("CameraSetup: Failed to allocate Ancillary buffers.\n");
			failed = true;
		}
        
    }
    
	return !failed;
}

// close camera, free memory.
void CameraUnsetup()
{
    tPvErr errCode;
	
    if((errCode = PvCameraClose(GCamera.Handle)) != ePvErrSuccess)
	{
		printf("CameraUnSetup: PvCameraClose err: %u\n", errCode);
	}
	else
	{
		printf("Camera closed.\n");
	}
	
	// delete image buffers
    for(int i=0;i<CAMERA_FRAMES_COUNT;i++){
        delete [] (char*)GCamera.Frames[i].ImageBuffer;
        delete [] (pvFrameAncillaryData*)GCamera.Frames[i].AncillaryBuffer;
    }
    
    GCamera.Handle = NULL;
    
}

// setup and start streaming
// return value: true == success, false == fail
bool CameraStart()
{
    tPvErr errCode;
    bool failed = false;
    
    // NOTE: This call sets camera PacketSize to largest sized test packet, up to 8228, that doesn't fail
	// on network card. Some MS VISTA network card drivers become unresponsive if test packet fails. 
	// Use PvUint32Set(handle, "PacketSize", MaxAllowablePacketSize) instead. See network card properties
	// for max allowable PacketSize/MTU/JumboFrameSize. 
	if((errCode = PvCaptureAdjustPacketSize(GCamera.Handle,8228)) != ePvErrSuccess)
	{
		printf("CameraStart: PvCaptureAdjustPacketSize err: %u\n", errCode);
		return false;
	}
    
    // start driver capture stream 
	if((errCode = PvCaptureStart(GCamera.Handle)) != ePvErrSuccess)
	{
		printf("CameraStart: PvCaptureStart err: %u\n", errCode);
		return false;
	}
	
    // queue frames with FrameDoneCB callback function. Each frame can use a unique callback function
	// or, as in this case, the same callback function.
	for(int i=0;i<CAMERA_FRAMES_COUNT && !failed;i++)
	{           
		if((errCode = PvCaptureQueueFrame(GCamera.Handle,&(GCamera.Frames[i]),FrameDoneCB)) != ePvErrSuccess)
		{
			printf("CameraStart: PvCaptureQueueFrame err: %u\n", errCode);
			failed = true;
		}
	}
    
	if (failed)
		return false;
    
	// set the camera in hardware trigger, continuous mode, and start camera receiving triggers
	if((PvAttrEnumSet(GCamera.Handle,"FrameStartTriggerMode","SyncIn1") != ePvErrSuccess) ||
       (PvAttrEnumSet(GCamera.Handle,"ExposureMode","External") != ePvErrSuccess) ||
       (PvAttrEnumSet(GCamera.Handle,"GainMode","Manual") != ePvErrSuccess) ||
       (PvAttrEnumSet(GCamera.Handle,"PixelFormat","Bayer12Packed") != ePvErrSuccess) ||
       (PvAttrEnumSet(GCamera.Handle,"AcquisitionMode","Continuous") != ePvErrSuccess) ||
       (PvCommandRun(GCamera.Handle,"AcquisitionStart") != ePvErrSuccess))
	{		
		printf("CameraStart: failed to set camera attributes\n");
		// clear queued frame
		PvCaptureQueueClear(GCamera.Handle);
		// stop driver capture stream
		PvCaptureEnd(GCamera.Handle);
		return false;
	}	
    
	return true;
}

// stop streaming
void CameraStop()
{
    tPvErr errCode;
	
	//stop camera receiving triggers
	if ((errCode = PvCommandRun(GCamera.Handle,"AcquisitionStop")) != ePvErrSuccess)
		printf("AcquisitionStop command err: %u\n", errCode);
	else
		printf("Camera stopped.\n");
    
    
    //PvCaptureQueueClear aborts any actively written frame with Frame.Status = ePvErrDataMissing
	//Further queued frames returned with Frame.Status = ePvErrCancelled
    
    //Add delay between AcquisitionStop and PvCaptureQueueClear
	//to give actively written frame time to complete
    
    printf("Waiting for frame to complete");
    for (int i = 0; i < 10; i++) {
        printf(".");
        usleep(100*1000);
    }
	
    //clear queued frames. will block until all frames dequeued
	if ((errCode = PvCaptureQueueClear(GCamera.Handle)) != ePvErrSuccess)
		printf("PvCaptureQueueClear err: %u\n", errCode);
	else
		printf("Queue cleared.\n");  
    
	//stop driver stream
	if ((errCode = PvCaptureEnd(GCamera.Handle)) != ePvErrSuccess)
		printf("PvCaptureEnd err: %u\n", errCode);
	else
		printf("Driver stream stopped.\n");
}

//Setup camera to stream. Spawn a thread to display stats.
void HandleCameraPlugged(unsigned long UniqueId)
{
    if(!GCamera.UID && !GCamera.Abort)
    {
        GCamera.UID = UniqueId;    
        
        if(CameraSetup())
        {
            printf("Camera %lu opened\n",UniqueId);   
            
            // start streaming from the camera
            if(CameraStart())
            {
                //create a thread to display camera stats. 
                SpawnThread();                    
            }
			else
			{
				//failure. signal main thread to abort
				GCamera.Abort = true;
			}
        }
        else
		{
            //failure. signal main thread to abort
			GCamera.Abort = true;
		}
    }  
}

void HandleCameraUnplugged(unsigned long UniqueId)
{
    if(GCamera.UID == UniqueId)
    {    
		//signal main thread to abort
		GCamera.Abort = true;
    }    
}

// callback function called on seperate thread when a registered camera event received
void CameraEventCB(void* Context,
                   tPvInterface Interface,
                   tPvLinkEvent Event,
                   unsigned long UniqueId)
{
    switch(Event)
    {
        case ePvLinkAdd:
        {
			printf("Event: camera %lu recognized\n",UniqueId);
            HandleCameraPlugged(UniqueId);
            break;
        }
        case ePvLinkRemove:
        {
			printf("\nEvent: camera %lu unplugged\n",UniqueId);
            HandleCameraUnplugged(UniqueId);
            
            break;
        }
        default:
            break;
    }
}

void PVAPIsetup(){
    
    tPvErr pvError;
    
    if((pvError = PvInitialize()) != ePvErrSuccess)
        printf("PvInitialize err: %u\n", pvError);
    else
    {
        //IMPORTANT: Initialize camera structure. See tPvFrame in PvApi.h for more info.
        memset(&GCamera,0,sizeof(tCamera));
        
        printf("Waiting for camera discovery...\n");
        
        // register camera plugged in callback
        if((pvError = PvLinkCallbackRegister(CameraEventCB,ePvLinkAdd,NULL)) != ePvErrSuccess)
            printf("PvLinkCallbackRegister err: %u\n", pvError);
        
        // register camera unplugged callback
        if((pvError = PvLinkCallbackRegister(CameraEventCB,ePvLinkRemove,NULL)) != ePvErrSuccess)
            printf("PvLinkCallbackRegister err: %u\n", pvError);
        
        PVAPIinitialised = true;
    }
}

#pragma mark Class methods


void lffCinderCaptureApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 1920, 1080 );
	settings->setFrameRate( -1 );
}

void lffCinderCaptureApp::setup()
{
    // init dynamic camera properties
    
    cameraWidth = 1920;
	cameraHeight = 1080;
    cameraGain = 0;
    
    cameraNumberBrackets = 5;
    cameraBracketEv = 4;
    cameraBaseExposure = 10ul;
	
    // init status vars
    
    justUpdatedBracket = 0;
    lastUpdatedBracket = 0;
    displayBracket = 0;
    
    theApp = this;
    
    setWindowPos(0, 20);
    setWindowSize(cameraWidth, cameraHeight);
    setFrameRate(-1);
    
    gl::enableDepthRead( false ); 
    gl::enableAlphaTest( false );
    gl::enableDepthWrite( false );
    gl::enableVerticalSync( false );
    
    // find the serial device
    
	const vector<Serial::Device> &devices( Serial::getDevices() );
	for( vector<Serial::Device>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt ) {
		// console() << "Device: " << deviceIt->getName() << endl;
	}
	
	try {
		Serial::Device dev = Serial::findDeviceByNameContains("tty.usbserial-A700exoq");
		serial = Serial( dev, 9600);
	}
	catch( ... ) {
		console() << "There was an error initializing the serial device!" << std::endl;
		quit();
	}
    
    serial.flush();
    
    usleep(1000*2000); // wait for the port to be ready
    
    //set camera parameters
    
    setCameraBracketEv(cameraBracketEv);
    setCameraNumberBrackets(cameraNumberBrackets);
    setCameraBaseExposure(cameraBaseExposure);
    printCameraParameters();    
    
    // load shader
    
	try {
		hdrDebayerShader = gl::GlslProg( loadResource( RES_DEBAYER_VERT ), loadResource( RES_DEBAYER_FRAG ) );
	}
	catch( gl::GlslProgCompileExc &exc ) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}
	catch( ... ) {
		std::cout << "Unable to load shader" << std::endl;
	}
    
    // make pixel buffers
    
    for(int i = 0;i<cameraNumberBrackets;i++){
        bracketBuffers.push_back(new BracketBuffer(cameraWidth, cameraHeight, i+1));
    }
    
    // make text overlays
    
    gl::Fbo::Format hdrFormat;
    hdrFormat.enableDepthBuffer( false );
    for(int i = 0;i<cameraNumberBrackets;i++){
        hdrFbos.push_back(gl::Fbo( cameraWidth, cameraHeight, hdrFormat ));
        TextLayout layout;
        layout.setFont( Font( "Lucida Grande", 16 ) );
        layout.setColor( Color( 1.0, 1.0, 1.0 ) );
        layout.addLine(str( boost::format("%1%") % (i+1) ));
        textTextures.push_back( gl::Texture( layout.render( true ) ) );
    }
    
    TextLayout layout;
    layout.setFont( Font( "Lucida Grande", 16 ) );
    layout.setColor( Color( 1.0, 1.0, 1.0 ) );
    layout.addLine("Waiting for camera");
    textTextures.push_back( gl::Texture( layout.render( true ) ) );
    
    // setup PVapi
    
    PVAPIinitialised = false;
    
    // Setup the parameters
	mParams = params::InterfaceGl( "Parameters", Vec2i( 175, 100 ) );
	mParams.addParam( "Gain", &cameraGain, "min=0 max=34 step=1 keyIncr=g keyDecr=G" );
    mParams.hide();
    
}

void lffCinderCaptureApp::keyDown( KeyEvent event )
{
	if( event.getCode() == app::KeyEvent::KEY_f ) {
		setFullScreen( ! isFullScreen() );
	}
    
    if( event.getCode() == app::KeyEvent::KEY_s ) {
        if(mParams.isVisible())
            mParams.hide();
        else
            mParams.show();
    }
    
    if( event.getCode() == app::KeyEvent::KEY_0 ) {
        displayBracket = 0;
    }
    if( event.getCode() == app::KeyEvent::KEY_1 ) {
        displayBracket = 1;
    }
    if( event.getCode() == app::KeyEvent::KEY_2 ) {
        displayBracket = 2;
    }
    if( event.getCode() == app::KeyEvent::KEY_3 ) {
        displayBracket = 3;
    }
    if( event.getCode() == app::KeyEvent::KEY_4 ) {
        displayBracket = 4;
    }
    if( event.getCode() == app::KeyEvent::KEY_5 ) {
        displayBracket = 5;
    }
    if( event.getCode() == app::KeyEvent::KEY_6 ) {
        displayBracket = 6;
    }
    if( event.getCode() == app::KeyEvent::KEY_7 ) {
        displayBracket = 7;
    }
    if( event.getCode() == app::KeyEvent::KEY_8 ) {
        displayBracket = 8;
    }
    if( event.getCode() == app::KeyEvent::KEY_9 ) {
        displayBracket = 9;
    }
    
    
    
}

void lffCinderCaptureApp::mouseDown( MouseEvent event )
{
    
}

void lffCinderCaptureApp::update()
{
    
    if(!PVAPIinitialised){
        
        PVAPIsetup();
        
    } else {
        
        if(GCamera.Abort){
            
            tPvErr errCode;
            
            CameraStop();
            CameraUnsetup();          
            
            // If thread spawned (see HandleCameraPlugged), wait to finish
            if(GCamera.ThHandle)
                WaitThread(); 
            
            if((errCode = PvLinkCallbackUnRegister(CameraEventCB,ePvLinkAdd)) != ePvErrSuccess)
                printf("PvLinkCallbackUnRegister err: %u\n", errCode);
            if((errCode = PvLinkCallbackUnRegister(CameraEventCB,ePvLinkRemove)) != ePvErrSuccess)
                printf("PvLinkCallbackUnRegister err: %u\n", errCode);       
            
            // uninitialize the API
            PvUnInitialize();
            PVAPIinitialised = false;
            
            GCamera.Abort = false;
            
        }
        
        if(GCamera.Handle){
            
            if(PvAttrUint32Set(GCamera.Handle,"GainValue",cameraGain) != ePvErrSuccess)
            {		
                printf("Camera: failed to set gain value\n");
            }
            
        }        
        
        int i = 0;
        
        for( vector<BracketBuffer*>::iterator iBuffer = bracketBuffers.begin(); iBuffer != bracketBuffers.end(); ++iBuffer ){
            bool wasUpdated = (*iBuffer)->needsUpdate;
            (*iBuffer)->update();
            if(wasUpdated){
                lastUpdatedBracket = justUpdatedBracket;
                justUpdatedBracket = i;
                renderBufferToFbo((*iBuffer), &hdrFbos[i]);
            }
            i++;
        }
        
        TextLayout layout;
        layout.setFont( Font( "Lucida Grande", 16 ) );
        layout.setColor( Color( 1.0, 1.0, 1.0 ) );
        layout.addLine(str( boost::format("Frames Completed: %1%") % GCamera.FrameCompleted ));
        layout.addLine(str( boost::format("Frames Dropped: %1%") % GCamera.FrameDropped ));
        layout.addLine(str( boost::format("Packets Received: %1%") % GCamera.PacketReceived ));
        layout.addLine(str( boost::format("Packets Missed: %1%") % GCamera.PacketMissed ));
        layout.addLine(str( boost::format("Camera Framerate: %1%") % GCamera.FrameRate ));
        layout.addLine(str( boost::format("Screen FPS: %1%") % getAverageFps() ));
        
        layout.addLine("Camera frames:" );
        textTextures[cameraNumberBrackets] = gl::Texture(layout.render( true ) );
        
    }
    
}

void lffCinderCaptureApp::draw()
{
    gl::setMatricesWindow( getWindowSize() );
    
    gl::clear( Color( 0.3, 0.3, 0.3 ) );
    gl::color(1.0, 1.0, 1.0);
    
	// use the scene we rendered into the FBO as a texture
	glEnable( GL_TEXTURE_2D );
    
    float windowWidth = ((getWindowWidth()-(10*(1+cameraNumberBrackets)))*(1.0/cameraNumberBrackets));
    float windowHeight = windowWidth*(1.0*cameraHeight/cameraWidth);
    
    int whichBracket = justUpdatedBracket;
    
    if(displayBracket > 0) whichBracket = min(displayBracket, cameraNumberBrackets)-1;
    
    gl::draw( hdrFbos[whichBracket].getTexture(0) );
    
    for(int i = 0; i < cameraNumberBrackets; i++){
        gl::color(1.0, 1.0, 1.0);
        gl::Texture myTexture(hdrFbos[i].getTexture(0));
        myTexture.setFlipped(true);
        gl::draw( myTexture, Rectf( 10+(i*(10+windowWidth)), 10, windowWidth+10+(i*(10+windowWidth)), 10+windowHeight) );
        gl::enableAlphaBlending( PREMULT );
        gl::color(0.0, 0.0, 0.0);
        gl::draw( textTextures[i], Vec2f( 21+(i*(10+windowWidth)), windowHeight-9 - (textTextures[i].getHeight()/2)));
        if (whichBracket == i) {
            gl::color(1.0, 1.0, 1.0);
        } else {
            gl::color(0.5, 0.5, 0.5);
        }
        gl::draw( textTextures[i], Vec2f( 20+(i*(10+windowWidth)), windowHeight-10 - (textTextures[i].getHeight()/2)));
        gl::disableAlphaBlending( );
    }
	glDisable( GL_TEXTURE_2D );
    
    
    // draw status text
    
    Vec2f textPos = Vec2f(10, getWindowHeight()-25);
    
    float bargraphsX = 120;
    
    gl::enableAlphaBlending( PREMULT );
    
    gl::color(0.0, 0.0, 0.0);
    gl::draw(textTextures[cameraNumberBrackets], textPos-Vec2f(-1,textTextures[cameraNumberBrackets].getHeight()-1));
    gl::color(1.0, 1.0, 1.0);
    gl::draw(textTextures[cameraNumberBrackets], textPos-Vec2f(0,textTextures[cameraNumberBrackets].getHeight()));
    
    gl::disableAlphaBlending();
    
    for (int i = 0; i < CAMERA_FRAMES_COUNT; i++){
        tPvFrame * aFrame;
        aFrame = &GCamera.Frames[i];
        if(aFrame->Context[0] == &gFramestateQueued){
            gl::color(0.1, 0.1, 0.1);
        } else {
            gl::color(1, 1, 1);
        }
        gl::drawSolidRect( Area((bargraphsX+15)+(i*10),getWindowHeight()-38,(bargraphsX+20)+(i*10),getWindowHeight()-28));
    }
    
    params::InterfaceGl::draw();
    
}

void lffCinderCaptureApp::processFrame( tPvFrame* pFrame )
{
    int bufferIndex = (pFrame->FrameCount-1) % cameraNumberBrackets;
    
    bracketBuffers[bufferIndex]->load(pFrame);
    
}

void lffCinderCaptureApp::renderBufferToFbo(BracketBuffer* buffer, gl::Fbo* fbo)
{
    
    // this will restore the old framebuffer binding when we leave this function
	// on non-OpenGL ES platforms, you can just call mFbo.unbindFramebuffer() at the end of the function
	// but this will restore the "screen" FBO on OpenGL ES, and does the right thing on both platforms
    
	gl::SaveFramebufferBinding bindingSaver;
    
    /**
     char * frameLabel = new char[20];
     std::sprintf(frameLabel, "Bracket %i", whichFbo);
     
     TextLayout layout;
     layout.setFont( Font( "Lucida Grande", 16 ) );
     layout.setColor( Color( 1.0, 1.0, 1.0 ) );
     layout.addLine(frameLabel);
     textTextures[whichFbo] = gl::Texture(layout.render( true ) ) ;
     
     **/
	// bind the framebuffer - now everything we draw will go there
	fbo->bindFramebuffer();
    
	// setup the viewport to match the dimensions of the FBO
	gl::setViewport( fbo->getBounds() );
    gl::setMatricesWindow( fbo->getSize() );
    
    renderHdrTexture(&(buffer->texture));
    
    // gl::draw(textTextures[whichFbo], hdrFbos[whichFbo].getBounds());
    
    fbo->unbindFramebuffer();
    
}

void lffCinderCaptureApp::renderHdrTexture(gl::Texture * tex){
    
    gl::clear( Color( 0, 0, 0 ) );
    
    gl::color(1.0, 1.0, 1.0);
    
    hdrDebayerShader.bind();
    tex->enableAndBind();
    
    hdrDebayerShader.uniform( "source", 0 );
    hdrDebayerShader.uniform( "sourceSize", Vec4f(cameraWidth, cameraHeight, 1.0/cameraWidth, 1.0/cameraHeight ) );
    hdrDebayerShader.uniform( "firstRed", Vec2f(1,0) );
    
    
    gl::drawSolidRect( Area(0,0,cameraWidth,cameraHeight));
    
    hdrDebayerShader.unbind();
    tex->unbind();
    tex->disable();
}

void lffCinderCaptureApp::setCameraNumberBrackets(int _numberBrackets){
    char _command[20];
    sprintf(_command, "N>%i\n",_numberBrackets);
    console() << sendSerialCommand(std::string(_command)) << endl;
}

void lffCinderCaptureApp::setCameraBracketEv(int _bracketEv){
    char _command[20];
    sprintf(_command, "V>%i\n",_bracketEv);
    console() << sendSerialCommand(std::string(_command)) << endl;
}

void lffCinderCaptureApp::setCameraBaseExposure(unsigned long _baseExposure){
    char _command[20];
    sprintf(_command, "E>%lu\n",_baseExposure);
    console() << sendSerialCommand(std::string(_command)) << endl;
}

void lffCinderCaptureApp::printCameraParameters(){
    char _command[20];
    sprintf(_command, "S>\n");
    console() << sendSerialCommand(std::string(_command)) << endl;
}

std::string lffCinderCaptureApp::sendSerialCommand(const std::string _command){
    
    std::string _returnString;
    // write command
    serial.writeString(_command);
    
    usleep(1000*1000);
    
    try{
        // read until newline, to a maximum of BUFSIZE bytes
        _returnString = serial.readStringUntil('!', SERIAL_BUFSIZE );
        
    } catch(SerialTimeoutExc e) {
        console() << "serial timeout" << endl;
    }
    
    //    console() << "\n" << _command << "COMMAND RETURNED:\n" << _returnString << endl;
    
    return _returnString;
    
    serial.flush();
    
}

lffCinderCaptureApp::~lffCinderCaptureApp(){
    tPvErr errCode;
    
    GCamera.Abort = true;
    
    CameraStop();
    CameraUnsetup();          
    
    // If thread spawned (see HandleCameraPlugged), wait to finish
    if(GCamera.ThHandle)
        WaitThread(); 
    
    if((errCode = PvLinkCallbackUnRegister(CameraEventCB,ePvLinkAdd)) != ePvErrSuccess)
        printf("PvLinkCallbackUnRegister err: %u\n", errCode);
    
    if((errCode = PvLinkCallbackUnRegister(CameraEventCB,ePvLinkRemove)) != ePvErrSuccess)
        printf("PvLinkCallbackUnRegister err: %u\n", errCode);       
    
    // uninitialize the API
    PvUnInitialize();
    PVAPIinitialised = false;
    
    GCamera.Abort = false;
    
    for( vector<BracketBuffer*>::iterator iBuffer = bracketBuffers.begin(); iBuffer != bracketBuffers.end(); ++iBuffer )
        delete *iBuffer;
    bracketBuffers.clear();
    
    
}

CINDER_APP_BASIC( lffCinderCaptureApp, RendererGl(0) )
