
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
#include "Logger.h"

#include <unistd.h>
#include <time.h>
#include <queue>
#include <math.h>

#include <dispatch/dispatch.h>
#include <ApplicationServices/ApplicationServices.h>

#include "BracketBuffer.h"
#include "Resources.h"


#pragma mark Defines

#define CAMERA_FRAMES_COUNT 50
#define CAMERA_STATUS_TEXT_SIZE 512

#define SERIAL_BUFSIZE 1024
#define SERIAL_READ_INTERVAL 0.25

static const bool PREMULT = false;

#pragma mark Namespaces

using namespace ci;
using namespace ci::app;
using namespace std;

#pragma mark Types

typedef struct {
    tPvFrame pFrame;
    boost::posix_time::ptime bTime;
} queuedFrame;

typedef struct {
    boost::filesystem::path filePath;
    float difference;
    unsigned long exposureMicros;
} frameData;

// Ancillary Data
typedef struct {
    tPvUint32 acquisitionCount;
    tPvUint32 userValue;
    tPvUint32 exposureValue;
    tPvUint32 gainValue;
    tPvUint32 syncInLevels;
    tPvUint32 syncOutLevels;
    tPvUint32 counter1value;
    tPvUint32 counter2value;
    tPvUint32 timer1value;
    tPvUint32 timer2value;
    tPvUint32 sequenceCount;
    tPvUint32 chunkID;
    tPvUint32 chunkLength;
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
    
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    void processFrame( tPvFrame* pFrame );
    void renderBeyerTextureToFbo(gl::Texture* tex, gl::Fbo* fbo);
    void renderHdrTexture(gl::Texture *tex);
    
    // frame history
    
    vector <frameData> frameHistory;
    
    // buffers
    
    dispatch_queue_t frameSerialQueue;
    dispatch_queue_t controllerSerialQueue;
    
    pthread_t frameProcessingThread;
    
    vector <BracketBuffer*> bracketBuffers;
    BracketBuffer * loadBuffer;
    
    vector <gl::Fbo> latestFBOs;
    vector <gl::Fbo> averageFBOs;
    
    float histogram[HISTOGRAM_BINS];
    int histogramMaxIndex;
    
    gl::GlslProg hdrDebayerShader;
    
    int justUpdatedBracket;
    int lastUpdatedBracket;
    
    int frameSaveInterval;
    int averageFrameSaveInterval;
    
    // serial stuff
    
    Serial serial;
    
    std::string sendSerialCommand(const std::string _command);
    
    void setCameraNumberBrackets(int _numberBrackets);
    void setCameraBracketEv(int _bracketEv);
    void setCameraBaseExposure(int _baseExposure);
    void resetBracketingSequenceCounter();
    void printCameraParameters();
    
    // camera
    
    int cameraWidth;
    int cameraHeight;
    
    int cameraGain;
    int cameraNumberBrackets;
    int cameraBracketEv;
    int cameraBaseExposure;
    
    // interface
    
    void keyDown( KeyEvent event );
    
    vector<gl::Texture>	textTextures;
    
    params::InterfaceGl	mParams;
    params::InterfaceGl	mStats;
    
    string statsCameraFrameComplete;
    string statsCameraFrameDropped;
    string statsCameraFrameRate;
    string statsCameraPacketReceived;
    string statsCameraPacketMissed;
    int statsFrameQueueSize;
    int statsCommandQueueSize;
    float statsAbsDifference;
    
    int displayBracket;
    bool showBracketBuffers;
    bool showFullscreen;
    bool showFullDisplay;
    bool showFullDisplayAverage;
    bool showMeters;
    
    bool autoExposure;
    float autoExposureSecondsInterval;
    double lastAutoExposureSeconds;
    
    int targetCameraNumberBrackets;
    int targetCameraBracketEv;
    int targetCameraGain;
    int targetCameraBaseExposure;
    
    float fps;
    
};

#pragma mark Globals

// global camera data
tCamera GCamera;
bool PVAPIinitialised;

// global app pointer
lffCinderCaptureApp * theApp;

unsigned long eventFrameTriggerCounter =0;
unsigned long eventExposureEndCounter =0;
unsigned long frameDoneCounter =0;
unsigned long frameProcessedCounter =0;

#pragma mark Global Functions

//Thread start function. Displays camera stats.
void *ThreadFunc(void *pContext)
{
    tPvErr Err;
    
    float lframeRate;
     
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
          ((Err = PvAttrFloat32Get(GCamera.Handle,"StatFrameRate",&lframeRate)) == ePvErrSuccess))
    {
        GCamera.FrameRate = (GCamera.FrameRate*0.9) + (lframeRate*0.1);
        usleep(100*1000);
	}
    
    return 0;
}

void F_CameraEventCallback(void*                   Context,
                           tPvHandle               Camera,
                           const tPvCameraEvent*	EventList,
                           unsigned long			EventListLength)
{
	//multiple events may have occurred for this one callback
	for (unsigned long i = 0; i < EventListLength; i++)
	{
		switch (EventList[i].EventId) {
			case 40000:
				LogStr("EventAcquisitionStart");
				break;
			case 40001:
				LogStr("EventAcquisitionEnd");
				break;
			case 40002:
                LogStr(str( (boost::format("%u EventFrameTrigger") % ++eventFrameTriggerCounter) ));
				break;
			case 40003:
                LogStr(str( (boost::format("%u EventExposureEnd") % ++eventExposureEndCounter) ));
				break;
			case 40004:
				LogStr("EventAcquisitionRecordTrigger");
				break;
			case 40010:
				LogStr("EventSyncIn1Rise");
				break;
			case 40011:
				LogStr("EventSyncIn1Fall");
				break;
			case 40012:
				LogStr("EventSyncIn2Rise");
				break;
			case 40013:
				LogStr("EventSyncIn2Fall");
				break;
			case 40014:
				LogStr("EventSyncIn3Rise");
				break;
			case 40015:
				LogStr("EventSyncIn3Fall");
				break;
			case 40016:
				LogStr("EventSyncIn4Rise");
				break;
			case 40017:
				LogStr("EventSyncIn4Fall");
				break;
			case 65534:
				LogStr("EventOverflow error");
				break;
			default:
                LogStr(str( (boost::format("Event %u") % EventList[i].EventId) ));
				break;
		}
	}
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
    LogStr("Waiting for a camera");
    while((PvCameraCount() == 0) && !GCamera.Abort)
    {
        LogStr(".");
        usleep(250*1000);
    }
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
    //LogStr(str( (boost::format("%u Frame %u Done") % ++frameDoneCounter % pFrame->FrameCount) ));
    
	// if frame hasn't been cancelled, requeue frame
    if(pFrame->Status != ePvErrCancelled){
        
        if(pFrame->Status == ePvErrSuccess)
        {
            pFrame->Context[0] = &gFramestateProcessing;
            theApp->processFrame(pFrame);
            //LogStr(str( (boost::format("%u Frame %u Procesed") % ++frameProcessedCounter % pFrame->FrameCount) ));
            
        }
        else
        {
            if (pFrame->Status == ePvErrDataMissing)
                LogStr("Dropped packets.");
            else
                LogStr(str( (boost::format("Frame.Status error: %u") % pFrame->Status) ));
        }
        pFrame->Context[0] = &gFramestateQueued;
        PvCaptureQueueFrame(GCamera.Handle,pFrame,FrameDoneCB);
    }
}

// setup event channel
// return value: true == success, false == fail
bool EventSetup()
{
	unsigned long EventBitmask;
	tPvErr errCode;
	
	// check if events supported with this camera firmware
	if (PvAttrExists(GCamera.Handle,"EventsEnable1") == ePvErrNotFound)
	{
        LogStr("This camera does not support event notifications.");
        return false;
	}
	
	//Clear all events
	//EventsEnable1 is a bitmask of all events. Bits correspond to last two digits of EventId.
	// e.g: Bit 1 is EventAcquisitionStart, Bit 2 is EventAcquisitionEnd, Bit 10 is EventSyncIn1Rise. 
    if ((errCode = PvAttrUint32Set(GCamera.Handle,"EventsEnable1",0)) != ePvErrSuccess)
	{
        LogStr(str( (boost::format("Set EventsEnable1 err: %u") % errCode) ));
		return false;
	}
    
    
	//Set individual events (could do in one step with EventsEnable1).
	if ((errCode = PvAttrEnumSet(GCamera.Handle,"EventSelector","AcquisitionStart")) != ePvErrSuccess)
	{
        LogStr(str( (boost::format("Set EventsSelector err: %u") % errCode) ));
		return false;
	}
    if ((errCode = PvAttrEnumSet(GCamera.Handle,"EventNotification","On")) != ePvErrSuccess)
	{
        LogStr(str( (boost::format("Set EventsNotification err: %u") % errCode) ));
		return false;
	}
    
	if ((errCode = PvAttrEnumSet(GCamera.Handle,"EventSelector","AcquisitionEnd")) != ePvErrSuccess)
	{
        LogStr(str( (boost::format("Set EventsSelector err: %u") % errCode) ));
		return false;
	}
    if ((errCode = PvAttrEnumSet(GCamera.Handle,"EventNotification","On")) != ePvErrSuccess)
	{
        LogStr(str( (boost::format("Set EventsNotification err: %u") % errCode) ));
		return false;
	}
    
	/*
     if ((errCode = PvAttrEnumSet(GCamera.Handle,"EventSelector","FrameTrigger")) != ePvErrSuccess)
     {
     LogStr(str( (boost::format("Set EventsSelector err: %u") % errCode) ));
     return false;
     }
     if ((errCode = PvAttrEnumSet(GCamera.Handle,"EventNotification","On")) != ePvErrSuccess)
     {
     LogStr(str( (boost::format("Set EventsNotification err: %u") % errCode) ));
     return false;
     }
     
     if ((errCode = PvAttrEnumSet(GCamera.Handle,"EventSelector","ExposureEnd")) != ePvErrSuccess)
     {
     LogStr(str( (boost::format("Set EventsSelector err: %u") % errCode) ));
     return false;
     }
     if ((errCode = PvAttrEnumSet(GCamera.Handle,"EventNotification","On")) != ePvErrSuccess)
     {
     LogStr(str( (boost::format("Set EventsNotification err: %u") % errCode) ));
     return false;
     }
     */
	
	//Get and print bitmask
	PvAttrUint32Get(GCamera.Handle,"EventsEnable1", &EventBitmask);
    LogStr(str( (boost::format("Events set. EventsEnable1 bitmask: %u") % EventBitmask) ));
    
    //register callback function
	if ((errCode = PvCameraEventCallbackRegister(GCamera.Handle,F_CameraEventCallback,NULL)) != ePvErrSuccess)
    {
        LogStr(str( (boost::format("PvCameraEventCallbackRegister err: %u") % errCode) ));
        return false;
    }     
	return true;
}

// unsetup event channel
void EventUnsetup()
{
    // wait so that the "AcquisitionEnd" [from CameraStop()] can be received on the event channel
    usleep(1000*1000);
	// clear all events
	PvAttrUint32Set(GCamera.Handle,"EventsEnable1",0);
    // unregister callback function
	PvCameraEventCallbackUnRegister(GCamera.Handle,F_CameraEventCallback);
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
			LogStr("PvCameraOpen returned ePvErrAccessDenied:\nCamera already open as Master, or camera wasn't properly closed and still waiting to HeartbeatTimeout.");
		else
            LogStr(str( (boost::format("PvCameraOpen err: %u") % errCode) ));
		return false;
	}
    
	// Activate Chunk Mode
    if((errCode = PvAttrBooleanSet(GCamera.Handle,"ChunkModeActive", TRUE)) != ePvErrSuccess)
	{
        LogStr(str( (boost::format("CameraSetup: Failed to activate Chunkmode: %u") % errCode) ));
		return false;
	}
    
	// Calculate frame buffer size
    if((errCode = PvAttrUint32Get(GCamera.Handle,"TotalBytesPerFrame",&FrameSize)) != ePvErrSuccess)
	{
        LogStr(str( (boost::format("CameraSetup: Get TotalBytesPerFrame err: %u") % errCode) ));
		return false;
	}
    
    // Calculate ancillary buffer size
    if((errCode = PvAttrUint32Get(GCamera.Handle,"NonImagePayloadSize",&AncillaryBufferSize)) != ePvErrSuccess)
	{
		
        LogStr(str( (boost::format("CameraSetup: Get NonImagePayloadSize err: %u") % errCode) ));
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
			LogStr("CameraSetup: Failed to allocate Frame buffers.");
			failed = true;
		}
        GCamera.Frames[i].AncillaryBuffer = new pvFrameAncillaryData;
        memset(GCamera.Frames[i].AncillaryBuffer, 0, sizeof(AncillaryBufferSize));
        
        if(GCamera.Frames[i].AncillaryBuffer)
        {
			GCamera.Frames[i].AncillaryBufferSize = AncillaryBufferSize;
		}
        else
		{
			LogStr("CameraSetup: Failed to allocate Ancillary buffers.");
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
        LogStr(str( (boost::format("CameraUnSetup: PvCameraClose err: %u") % errCode) ));
	}
	else
	{
		LogStr("Camera closed.");
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
        LogStr(str( (boost::format("CameraStart: PvCaptureAdjustPacketSize err: %u") % errCode) ));
		return false;
	}
    
    // start driver capture stream 
	if((errCode = PvCaptureStart(GCamera.Handle)) != ePvErrSuccess)
	{
        LogStr(str( (boost::format("CameraStart: PvCaptureStart err: %u") % errCode) ));
		return false;
	}
	
    // queue frames with FrameDoneCB callback function. Each frame can use a unique callback function
	// or, as in this case, the same callback function.
	for(int i=0;i<CAMERA_FRAMES_COUNT && !failed;i++)
	{           
		if((errCode = PvCaptureQueueFrame(GCamera.Handle,&(GCamera.Frames[i]),FrameDoneCB)) != ePvErrSuccess)
		{
            LogStr(str( (boost::format("CameraStart: PvCaptureQueueFrame err: %u") % errCode) ));
			failed = true;
		}
	}
    
	if (failed)
		return false;
    
	// set the camera in hardware trigger, continuous mode, and start camera receiving triggers
	if((PvAttrEnumSet(GCamera.Handle,"FrameStartTriggerMode","SyncIn1") != ePvErrSuccess) ||
       (PvAttrUint32Set(GCamera.Handle,"FrameStartTriggerDelay",0) != ePvErrSuccess) ||
       (PvAttrEnumSet(GCamera.Handle,"ExposureMode","External") != ePvErrSuccess) ||
       (PvAttrEnumSet(GCamera.Handle,"GainMode","Manual") != ePvErrSuccess) ||
       (PvAttrEnumSet(GCamera.Handle,"PixelFormat","Bayer12Packed") != ePvErrSuccess) ||
       (PvAttrEnumSet(GCamera.Handle,"AcquisitionMode","Continuous") != ePvErrSuccess) ||
       (PvCommandRun(GCamera.Handle,"AcquisitionStart") != ePvErrSuccess))
	{		
 		LogStr("CameraStart: failed to set camera attributes");
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
        LogStr(str( (boost::format("AcquisitionStop command err: %u") % errCode) ));
	else
		LogStr("Camera stopped.");
    
    
    //PvCaptureQueueClear aborts any actively written frame with Frame.Status = ePvErrDataMissing
	//Further queued frames returned with Frame.Status = ePvErrCancelled
    
    //Add delay between AcquisitionStop and PvCaptureQueueClear
	//to give actively written frame time to complete
    
    LogStr("Camera stopped.");
    for (int i = 0; i < 10; i++) {
        LogStr(".");
        usleep(100*1000);
    }
	
    //clear queued frames. will block until all frames dequeued
	if ((errCode = PvCaptureQueueClear(GCamera.Handle)) != ePvErrSuccess)
        LogStr(str( (boost::format("PvCaptureQueueClear err: %u") % errCode) ));
	else
		LogStr("Queue cleared.");  
    
	//stop driver stream
	if ((errCode = PvCaptureEnd(GCamera.Handle)) != ePvErrSuccess)
        LogStr(str( (boost::format("PvCaptureEnd err: %u") % errCode) ));
	else
		LogStr("Driver stream stopped.");
}

//Setup camera to stream. Spawn a thread to display stats.
void HandleCameraPlugged(unsigned long UniqueId)
{
    if(!GCamera.UID && !GCamera.Abort)
    {
        GCamera.UID = UniqueId;    
        
        if(CameraSetup())
        {
            LogStr(str( (boost::format("Camera %lu opened") % UniqueId) ));
            
            // setup event channel
            if(EventSetup())
			{
                
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
            LogStr(str( (boost::format("Event: camera %lu recognized") % UniqueId) ));
            HandleCameraPlugged(UniqueId);
            break;
        }
        case ePvLinkRemove:
        {
            LogStr(str( (boost::format("Event: camera %lu unplugged") % UniqueId) ));
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
        LogStr(str( (boost::format("PvInitialize err: %u") % pvError) ));
    else
    {
        //IMPORTANT: Initialize camera structure. See tPvFrame in PvApi.h for more info.
        memset(&GCamera,0,sizeof(tCamera));
        
        LogStr("Waiting for camera discovery...");
        
        // register camera plugged in callback
        if((pvError = PvLinkCallbackRegister(CameraEventCB,ePvLinkAdd,NULL)) != ePvErrSuccess)
            LogStr(str( (boost::format("PvLinkCallbackRegister err: %u") % pvError) ));
        
        // register camera unplugged callback
        if((pvError = PvLinkCallbackRegister(CameraEventCB,ePvLinkRemove,NULL)) != ePvErrSuccess)
            LogStr(str( (boost::format("PvLinkCallbackRegister err: %u") % pvError) ));
        
        PVAPIinitialised = true;
    }
}


#pragma mark Class methods


void lffCinderCaptureApp::setup()
{
    // init dynamic camera properties
    
    cameraWidth = 1920;
    cameraHeight = 1080;
    
    cameraGain = targetCameraGain = 0;
    cameraNumberBrackets = targetCameraNumberBrackets = 1;
    cameraBracketEv = targetCameraBracketEv = 3;
    cameraBaseExposure = targetCameraBaseExposure = 1500;
    
    autoExposure = true;
    autoExposureSecondsInterval = 10.0;
    lastAutoExposureSeconds = 0.0;
    
    // init status vars
    
    justUpdatedBracket = 0;
    lastUpdatedBracket = 0;
    displayBracket = 1;
    
    showBracketBuffers = false;
    showFullscreen = false;
    showFullDisplay = true;
    showFullDisplayAverage = false;
    showMeters = false;
    
    frameSaveInterval = 200;
    averageFrameSaveInterval = 200;
    
    PVAPIinitialised = false;
    
    theApp = this;
    
    setWindowPos(0, 20);
    setWindowSize(cameraWidth, cameraHeight*0.8);
    setFrameRate(5);
    
    gl::enableDepthRead( false ); 
    gl::enableAlphaTest( false );
    gl::enableDepthWrite( false );
    gl::enableVerticalSync( true );
    
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
    
    loadBuffer = new BracketBuffer(cameraWidth, cameraHeight, 0);
    
    for(int i = 0;i<cameraNumberBrackets;i++){
        bracketBuffers.push_back(new BracketBuffer(cameraWidth, cameraHeight, i+1));
    }
    
    // make text overlays
    
    gl::Fbo::Format hdrFormat;
    hdrFormat.enableDepthBuffer( false );
    for(int i = 0;i<cameraNumberBrackets;i++){
        latestFBOs.push_back(gl::Fbo( cameraWidth, cameraHeight, hdrFormat ));
        averageFBOs.push_back(gl::Fbo( cameraWidth, cameraHeight, hdrFormat ));
        TextLayout layout;
        layout.setFont( Font( "Lucida Grande", 16 ) );
        layout.setColor( Color( 1.0, 1.0, 1.0 ) );
        layout.addLine(str( boost::format("%1%") % (i+1) ));
        textTextures.push_back( gl::Texture( layout.render( true ) ) );
    }
    
    // Parameter interface
    
	mParams = params::InterfaceGl( "Parameters", Vec2i( 400, 300 ) );
	mParams.addText( "labelCamera", "label=`Camera`" );
    mParams.addParam( "Auto Exposure", &autoExposure, "key=x" );
    mParams.addParam( "Auto Exposure Interval Sec", &autoExposureSecondsInterval, "min=0.1 max=120 step=0.1 keyIncr=I keyDecr=i" );
    mParams.addParam( "Base Exposure", &targetCameraBaseExposure, "min=1 max=500000 step=10 keyIncr=E keyDecr=e" );
	mParams.addParam( "Gain", &targetCameraGain, "min=0 max=34 step=1 keyIncr=G keyDecr=g" );
    mParams.addParam( "Bracket EV Steps", &targetCameraBracketEv, "min=1 max=4 step=1 keyIncr=V keyDecr=v" );
	// mParams.addParam( "Number Brackets", &targetCameraNumberBrackets, "min=1 max=9 step=1 keyIncr=N keyDecr=n" );
    mParams.addSeparator();	
	mParams.addText( "labelSaving", "label=`Saving`" );
	mParams.addParam( "Latest Interval", &frameSaveInterval, "min=0 max=10000 step=1 keyIncr=L keyDecr=l" );
	mParams.addParam( "Average Interval", &averageFrameSaveInterval, "min=0 max=10000 step=1 keyIncr=A keyDecr=a" );
    mParams.addSeparator();	
	mParams.addText( "labelDisplay", "label=`Display`" );
	mParams.addParam( "Full Screen", &showFullscreen, "key=f" );
	mParams.addParam( "Full Display", &showFullDisplay,  "key=d" );
	mParams.addParam( "Full Display is Average", &showFullDisplayAverage, "key=D" );
    mParams.addParam( "Full Display Bracket Index", &displayBracket, str( boost::format("min=0 max=%1% step=1 keyIncr=g keyDecr=G") %  cameraNumberBrackets ));
	mParams.addParam( "Buffers", &showBracketBuffers,  "key=b"  );
	mParams.addParam( "Meters", &showMeters,  "key=m"  );
    
    // Stats interface
    
    statsCameraFrameComplete = "0";
    statsCameraFrameDropped = "0";
    statsCameraFrameRate = "0";
    statsCameraPacketReceived = "0";
    statsCameraPacketMissed = "0";
    statsFrameQueueSize = 0;
    statsCommandQueueSize = 0;
    statsAbsDifference = 0.0;
    
    mStats = params::InterfaceGl( "Stats", Vec2i( 400, 300 ) );
    mStats.setOptions("", str( boost::format("position='%1% %2%' refresh=0.1") % 425 % 15 ));
    mStats.addText( "textFrames", "label=`Camera Frames`" );
    mStats.addParam( "Complete", &statsCameraFrameComplete, "readonly=true" );
    mStats.addParam( "Dropped", &statsCameraFrameDropped, "readonly=true" );
    mStats.addParam( "Rate", &statsCameraFrameRate, "readonly=true" );
    mStats.addParam( "Frame Queue", &statsFrameQueueSize, "readonly=true" );
    mStats.addParam( "Difference", &statsAbsDifference, "readonly=true" );
    mStats.addText( "textCamera", "label=`Camera Packets`" );
    mStats.addParam( "Received", &statsCameraPacketReceived, "readonly=true" );
    mStats.addParam( "Missed", &statsCameraPacketMissed, "readonly=true" );
    mStats.addSeparator();
    mStats.addText( "textSerial", "label=`Serial communication`" );
    mStats.addParam( "Command Queue", &statsCommandQueueSize, "readonly=true" );
    mStats.addSeparator();
    mStats.addText( "textDisplay", "label=`Display`" );
    mStats.addParam( "FPS", &fps, "readonly=true" );
    
    frameSerialQueue = dispatch_queue_create("gl.longing.frameQueue", NULL);
    controllerSerialQueue = dispatch_queue_create("gl.longing.controllerQueue", NULL);
    
    //mParams.hide();
    
}

void lffCinderCaptureApp::keyDown( KeyEvent event )
{
    
    if( event.getCode() == app::KeyEvent::KEY_TAB ) {
        if(mParams.isVisible()){
            mParams.hide();
            mStats.hide();
        } else {
            mParams.show();
            mStats.show();
        }
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
    fps = getAverageFps();
    
    if( isFullScreen() !=  showFullscreen) {
		setFullScreen(showFullscreen);
	}
    
    
    if(!PVAPIinitialised){
        
        PVAPIsetup();
        
    } else {
        
        if(GCamera.Abort){
            
            tPvErr errCode;
            
            CameraStop();
            EventUnsetup();
            CameraUnsetup();          
            
            // If thread spawned (see HandleCameraPlugged), wait to finish
            if(GCamera.ThHandle)
                WaitThread(); 
            
            if((errCode = PvLinkCallbackUnRegister(CameraEventCB,ePvLinkAdd)) != ePvErrSuccess)
                LogStr(str( (boost::format("PvLinkCallbackUnRegister err: %u") % errCode) ));
            if((errCode = PvLinkCallbackUnRegister(CameraEventCB,ePvLinkRemove)) != ePvErrSuccess)
                LogStr(str( (boost::format("PvLinkCallbackUnRegister err: %u") % errCode) ));
            
            // uninitialize the API
            PvUnInitialize();
            PVAPIinitialised = false;
            
            GCamera.Abort = false;
            
            resetBracketingSequenceCounter();
            
        }
        
        int i = 0;
        
        float lAbsDifference = 0.0;
        
        for( vector<BracketBuffer*>::iterator iBuffer = bracketBuffers.begin(); iBuffer != bracketBuffers.end(); ++iBuffer ){
            bool wasUpdated = (*iBuffer)->needsUpdate;
            (*iBuffer)->update();
            if(wasUpdated){
                if (i==0) {
                    cblas_scopy(HISTOGRAM_BINS, (*iBuffer)->histogram,1, histogram,1);
                    lAbsDifference = (*iBuffer)->absDifference;
                    histogramMaxIndex = (*iBuffer)->histogramMaxIndex;
                }
                lastUpdatedBracket = justUpdatedBracket;
                justUpdatedBracket = i;
                
                if (showBracketBuffers || (showFullDisplay && (displayBracket == i+1 || displayBracket == 0))) {
                    if (showBracketBuffers || !showFullDisplayAverage) {
                        renderBeyerTextureToFbo(&(*iBuffer)->texture, &latestFBOs[i]);
                    }
                    if (showBracketBuffers || showFullDisplayAverage ) {
                        renderBeyerTextureToFbo(&(*iBuffer)->averageTexture, &averageFBOs[i]);
                    }
                }
            }
            i++;
        }
        
        if(GCamera.Handle){
            if(cameraGain != targetCameraGain){
                if(PvAttrUint32Set(GCamera.Handle,"GainValue",targetCameraGain) != ePvErrSuccess)
                {		
                    LogStr("Camera: failed to set gain value");
                } else {
                    cameraGain = targetCameraGain;
                }
            }
            
            //TODO: Camera target parametre opdateres
            
            //Auto base exposure
            if(autoExposure){
                
                int newTargetCameraBaseExposure = targetCameraBaseExposure;
                
                if(getElapsedSeconds() - lastAutoExposureSeconds > autoExposureSecondsInterval){
                    
                    float histogramEnd = histogram[HISTOGRAM_BINS-1];
                    
                    int targetCameraBaseExposureOffset = MAX(1, newTargetCameraBaseExposure*0.01);
                    
                    if (histogramEnd > 0.0005) {
                        // overexposed
                        newTargetCameraBaseExposure-=targetCameraBaseExposureOffset;
                    } 
                    if (histogramEnd < 0.0003) {
                        // underexposed
                        newTargetCameraBaseExposure+=targetCameraBaseExposureOffset;
                    }
                    if(newTargetCameraBaseExposure >= 30 && newTargetCameraBaseExposure <= 60000000 ){
                        targetCameraBaseExposure = newTargetCameraBaseExposure;
                    }
                    lastAutoExposureSeconds = getElapsedSeconds();
                }
            }
            
            if(cameraBracketEv != targetCameraBracketEv){
                setCameraBracketEv(targetCameraBracketEv);
                cameraBracketEv = targetCameraBracketEv;
            }
            
            if(cameraNumberBrackets != targetCameraNumberBrackets){
                setCameraNumberBrackets(targetCameraNumberBrackets);
                cameraNumberBrackets = targetCameraNumberBrackets;
            }
            
            if(cameraBaseExposure != targetCameraBaseExposure){
                statsCommandQueueSize++;
                dispatch_async(controllerSerialQueue, ^{
                    setCameraBaseExposure(targetCameraBaseExposure);
                    statsCommandQueueSize--;
                });
                cameraBaseExposure = targetCameraBaseExposure;
            }
            
        }        
        
        
        std::string _returnString = "";
        
        if(serial.getNumBytesAvailable() > 20){
            
            try{
                // read until newline, to a maximum of BUFSIZE bytes
                _returnString = serial.readStringUntil('!', SERIAL_BUFSIZE, 0.01 );
                _returnString += serial.readStringUntil('\n', SERIAL_BUFSIZE, 0.01);
                // LogStr(_returnString);
                
            } catch(SerialTimeoutExc e) {
                LogStr("serial timeout");
            }
        }

        
        // update stats
        
        statsCameraFrameComplete = str(boost::format("%1%") % GCamera.FrameCompleted );
        statsCameraFrameDropped = str(boost::format("%1%") % GCamera.FrameDropped );
        statsCameraFrameRate = str(boost::format("%1%") % GCamera.FrameRate );
        statsCameraPacketReceived = str(boost::format("%1%") % GCamera.PacketReceived );
        statsCameraPacketMissed = str(boost::format("%1%") % GCamera.PacketMissed );
        statsAbsDifference = lAbsDifference;
        
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
    
    // full screen
    
    if(showFullDisplay){
        if (showFullDisplayAverage) {
            gl::Texture averageTexture(averageFBOs[whichBracket].getTexture(0));
            averageTexture.setFlipped(true);
            gl::draw( averageTexture, Rectf(0,0,getWindowWidth(),getWindowWidth()*(1.0*cameraHeight/cameraWidth) ));
        } else { 
            gl::Texture latestTexture(latestFBOs[whichBracket].getTexture(0));
            latestTexture.setFlipped(true);
            gl::draw( latestTexture, Rectf(0,0,getWindowWidth(),getWindowWidth()*(1.0*cameraHeight/cameraWidth) ));
        }
    }
    // bracket buffers
    
    if(showBracketBuffers){
        
        for(int i = 0; i < cameraNumberBrackets; i++){
            gl::color(1.0, 1.0, 1.0);
            gl::Texture latestTexture(latestFBOs[i].getTexture(0));
            latestTexture.setFlipped(true);
            gl::draw( latestTexture, Rectf( 10+(i*(10+windowWidth)), 10, windowWidth+10+(i*(10+windowWidth)), 10+windowHeight) );
            gl::Texture averageTexture(averageFBOs[i].getTexture(0));
            averageTexture.setFlipped(true);
            gl::draw( averageTexture, Rectf( 10+(i*(10+windowWidth)), 10+windowHeight+10, windowWidth+10+(i*(10+windowWidth)), (10+windowHeight)*2) );
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
    }
    
    glDisable( GL_TEXTURE_2D );
    
    if (showMeters){
        
        // difference meter
        
        float absDiffScaled = sinf(statsAbsDifference*1.570796326794897);
        
        gl::enableAlphaBlending();
        
        gl::color(0.1, 0.1, 0.1, 0.33);
        gl::drawSolidRect( Area(10,getWindowHeight()-30,(getWindowWidth()-10),getWindowHeight()-10));
        
        gl::color(1.0, 1.0, 1.0, 0.5);
        gl::drawSolidRect( Area(10,getWindowHeight()-30,(getWindowWidth()-10)*absDiffScaled,getWindowHeight()-10));
        
        // histogram
        
        Area histogramArea = Area(
                                  10,getWindowHeight()-40, getWindowWidth()-10, (getWindowHeight()-40)*0.66
                                  );
        
        gl::color(0.1, 0.1, 0.1, 0.33);
        gl::drawSolidRect(histogramArea);
        
        gl::color(1.0, 1.0, 1.0,0.5);
        
        for(int i=0; i<HISTOGRAM_BINS; i++){
            float w = 1.0*histogramArea.getWidth()/HISTOGRAM_BINS;
            float h = (histogram[i]/histogram[histogramMaxIndex])*histogramArea.getHeight();
            gl::drawSolidRect(Area(10+(w*i),histogramArea.y2-h,10+(w*i)+w,histogramArea.y2));
        }
        
        gl::disableAlphaBlending();
        
    }
    
    params::InterfaceGl::draw();
    
}

void lffCinderCaptureApp::processFrame( tPvFrame* pFrame )
{
    
    queuedFrame * qFrame = new queuedFrame;
    
    qFrame->pFrame.ImageBufferSize = pFrame->ImageBufferSize;
    qFrame->pFrame.Status = pFrame->Status;
    qFrame->pFrame.ImageSize = pFrame->ImageSize;
    qFrame->pFrame.Width = pFrame->Width;
    qFrame->pFrame.Height = pFrame->Height;
    qFrame->pFrame.RegionX = pFrame->RegionX;
    qFrame->pFrame.RegionY = pFrame->RegionY;
    qFrame->pFrame.Format = pFrame->Format;
    qFrame->pFrame.BitDepth = pFrame->BitDepth;
    qFrame->pFrame.BayerPattern = pFrame->BayerPattern;
    qFrame->pFrame.FrameCount = pFrame->FrameCount;
    qFrame->pFrame.TimestampLo = pFrame->TimestampLo;
    qFrame->pFrame.TimestampHi = pFrame->TimestampHi;
    
    qFrame->pFrame.ImageBuffer = new char*[pFrame->ImageBufferSize];
    
    memcpy(qFrame->pFrame.ImageBuffer, pFrame->ImageBuffer, pFrame->ImageBufferSize);
    
    //  pvFrameAncillaryData* data = (pvFrameAncillaryData*)pFrame->AncillaryBuffer;
    //  console() << "Frame " << pFrame->FrameCount << " \t : " <<  data->exposureValue << endl;
    
    boost::posix_time::ptime uTime = boost::posix_time::microsec_clock::universal_time();
    
    qFrame->bTime = uTime;
    
    statsFrameQueueSize++;
    
    dispatch_async(frameSerialQueue, ^{
                
        tPvFrame pFrame = qFrame->pFrame;
        
        tm tm = boost::posix_time::to_tm(qFrame->bTime);
        boost::posix_time::time_duration duration( qFrame->bTime.time_of_day() );
        int uTimeMillis = duration.total_milliseconds() %1000;
        
        loadBuffer->load(&pFrame);
        
        unsigned long frameCount = pFrame.FrameCount;
        
        int bufferIndex = (frameCount-1) % theApp->cameraNumberBrackets;
        int bracektedFrameNumber = ((frameCount-1)/theApp->cameraNumberBrackets);
                
        float brightnessDifference = 1.0;
        
        if(frameCount > cameraNumberBrackets*2){
            for(int i = 0; i < theApp->cameraNumberBrackets; i++){
                float iDiff = fabs(loadBuffer->brightness-bracketBuffers[i]->brightness);
                if(iDiff < brightnessDifference){
                    brightnessDifference = iDiff;
                    bufferIndex = i;
                }
            }
        }
        
        bracketBuffers[bufferIndex]->updateFrom(loadBuffer);
                
        delete (USHORT*) pFrame.ImageBuffer;
        
        delete qFrame;
        
        if(frameSaveInterval > 0){
            if(bracektedFrameNumber % frameSaveInterval == 0) {
                dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
                    
                    bracketBuffers[bufferIndex]->saveFrame(
                                                           str( (boost::format("/Users/ole/Pictures/Captures/%3$04d-%4$02d-%5$02d/%3$04d-%4$02d-%5$02d_%7$02d-%8$02d-%9$02d-%10$04d_c%6$08d_s%1$06d-b%2$1d_d%11%.bayer16") % bracektedFrameNumber % bufferIndex % (1900 + tm.tm_year) % (tm.tm_mon+1) % tm.tm_mday % frameCount % tm.tm_hour % tm.tm_min % tm.tm_sec % uTimeMillis % bracketBuffers[bufferIndex]->absDifference) ).c_str() 
                                                           );
                });
            }
        }
        if(averageFrameSaveInterval > 0){
            
            if(bracektedFrameNumber % averageFrameSaveInterval == 0) {
                dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
                    
                    bracketBuffers[bufferIndex]->saveAverageFrame(
                                                                  str( (boost::format("/Users/ole/Pictures/Captures/%3$04d-%4$02d-%5$02d/%3$04d-%4$02d-%5$02d_%7$02d-%8$02d-%9$02d-%10$04d_c%6$08d-s%1$06d-b%2$1d_d%11%_average.bayer16") % bracektedFrameNumber % bufferIndex % (1900 + tm.tm_year) % (tm.tm_mon+1) % tm.tm_mday % frameCount % tm.tm_hour % tm.tm_min % tm.tm_sec % uTimeMillis % bracketBuffers[bufferIndex]->absDifferenceAverage ) ).c_str() 
                                                                  );
                    bracketBuffers[bufferIndex]->framesAddedToAverage = MIN(averageFrameSaveInterval,bracektedFrameNumber);
                });
            }
        }
        
        statsFrameQueueSize--;
        
    });    
    
}

void lffCinderCaptureApp::renderBeyerTextureToFbo(gl::Texture * tex, gl::Fbo* fbo)
{
    
    // this will restore the old framebuffer binding when we leave this function
	// on non-OpenGL ES platforms, you can just call mFbo.unbindFramebuffer() at the end of the function
	// but this will restore the "screen" FBO on OpenGL ES, and does the right thing on both platforms
    
	gl::SaveFramebufferBinding bindingSaver;
    
	// bind the framebuffer - now everything we draw will go there
	fbo->bindFramebuffer();
    
	// setup the viewport to match the dimensions of the FBO
	gl::setViewport( fbo->getBounds() );
    gl::setMatricesWindow( fbo->getSize() );
    
    renderHdrTexture(tex);
    
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
    string output = sendSerialCommand(str( (boost::format("N>%i\n") % _numberBrackets) ));
    //LogStr(output);
}

void lffCinderCaptureApp::resetBracketingSequenceCounter(){
    string output = sendSerialCommand("0>\n");
    //LogStr(output);
}

void lffCinderCaptureApp::setCameraBracketEv(int _bracketEv){
    string output = sendSerialCommand(str( (boost::format("V>%i\n") % _bracketEv) ));
    //LogStr(output);
}

void lffCinderCaptureApp::setCameraBaseExposure(int _baseExposure){
    string output = sendSerialCommand(str( (boost::format("T>%i\n") % _baseExposure) ));
    //LogStr(output);
}

void lffCinderCaptureApp::printCameraParameters(){
    LogStr(sendSerialCommand("S>\n"));
}

std::string lffCinderCaptureApp::sendSerialCommand(const std::string _command){
    
    // write command
    serial.writeString(_command);
        
    return str( (boost::format("Serial command: \"%s\"\n") % boost::algorithm::replace_all_copy(_command, "\n", "\\n")) );
    
}

lffCinderCaptureApp::~lffCinderCaptureApp(){
    tPvErr errCode;
    
    GCamera.Abort = true;
    
    CameraStop();
    EventUnsetup();
    CameraUnsetup();          
    
    // If thread spawned (see HandleCameraPlugged), wait to finish
    if(GCamera.ThHandle)
        WaitThread(); 
    
    if (frameProcessingThread) {
        pthread_join(frameProcessingThread,NULL);
    }
    
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
