//
//  BracketBuffer.cpp
//  lffCinderCapture
//
//  Created by Ole Kristensen on 13/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <sstream>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include "Logger.h"
#include "zlib.h"

#include <CoreServices/CoreServices.h>

#include "BracketBuffer.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Texture.h"

BracketBuffer::BracketBuffer(int _width, int _height, int _bracketIndex){
    
    K = 1.0 / 0xFFF;
    
    frameNumber = 0;
    framesAddedToAverage = 0;
    needsUpdate = true;
    absDifference = 0;
    histogramMax = 1.0;
    histogramMaxIndex = 0;
    exposureMicroseconds = 0;
    exposureMicrosecondsAverage = 0;
    
    for(int i=0;i<HISTOGRAM_BINS;i++){
        histogram[i] = 0.0;
    }
    histogram[0] = 1.0;
    
    brightness = 0.0;
    
    bracketIndex = _bracketIndex;
    width = _width;
    height = _height;
    
    
    // init latest frame
    
    pixels16U = new USHORT [width * height];
    memset(pixels16U, 0xFFFF, width*height);
    vImage16U.data = pixels16U;
    vImage16U.width = width;
    vImage16U.height = height;
    vImage16U.rowBytes = width*2;
    
    pixelsFloat = new float [width * height];
    memset(pixelsFloat, 0.0, width*height);
    vImageFloat.data = pixelsFloat;
    vImageFloat.width = width;
    vImageFloat.height = height;
    vImageFloat.rowBytes = width*4;
    
    channel = cinder::Channel32f(vImageFloat.width, vImageFloat.height, vImageFloat.rowBytes, 1, pixelsFloat);
    texture = cinder::gl::Texture(channel);
    
    
    // init former frame
    
    formerPixelsFloat = new float [width * height];
    memset(formerPixelsFloat, 0.0, width*height);
    formerVImageFloat.data = formerPixelsFloat;
    formerVImageFloat.width = width;
    formerVImageFloat.height = height;
    formerVImageFloat.rowBytes = width*2;
    
    
    // init average frame
    
    averagePixels16U = new USHORT [width * height];
    memset(averagePixels16U, 0xFFFF, width*height);
    averageVImage16U.data = averagePixels16U;
    averageVImage16U.width = width;
    averageVImage16U.height = height;
    averageVImage16U.rowBytes = width*2;
    
    averagePixelsFloat = new float [width * height];
    memset(averagePixelsFloat, 0.0, width*height);
    averageVImageFloat.data = averagePixelsFloat;
    averageVImageFloat.width = width;
    averageVImageFloat.height = height;
    averageVImageFloat.rowBytes = width*4;
    
    averageChannel = cinder::Channel32f(averageVImageFloat.width, averageVImageFloat.height, averageVImageFloat.rowBytes, 1, averagePixelsFloat);
    averageTexture = cinder::gl::Texture(averageChannel);
    
    
    
}

void BracketBuffer::load(tPvFrame * pFrame)
{
    
    // make temporary frame
    
    pvFrame = *pFrame;
    
    pvFrame.Format           = ePvFmtBayer16;
    pvFrame.ImageBufferSize  = pFrame->Width * pFrame->Height * 2;
    pvFrame.ImageBuffer      = pixels16U;
    
    if(pvFrame.ImageBuffer)
    {
        
        // conversion from packed 12 bit to 16 bit planar
        
        const Packed12BitsPixel_t*  pSrc = (const Packed12BitsPixel_t*)pFrame->ImageBuffer;
        const Packed12BitsPixel_t*  pSrcEnd = (const Packed12BitsPixel_t*)((unsigned char*)pFrame->ImageBuffer + pFrame->ImageSize);
        USHORT*				        pDest = (USHORT*)pvFrame.ImageBuffer;
        USHORT*                     pDestEnd = (USHORT*)pvFrame.ImageBuffer + pFrame->Width * pFrame->Height;
        //        const ULONG			        bitshift = 16 - pFrame->BitDepth;
        USHORT                      pixel1,pixel2;
        
        while (pSrc < pSrcEnd && pDest < pDestEnd)
        {
            for (ULONG i = 0; i < pFrame->Width && pSrc < pSrcEnd; i+=2)
            {
                pixel1 = (USHORT)pSrc->LByte << 4;
                pixel1 += ((USHORT)pSrc->MByte & 0xF0) >> 4;
                
                pixel2 = (USHORT)pSrc->UByte << 4;
                pixel2 += ((USHORT)pSrc->MByte & 0x0F) >> 4;
                
                if(pDest < pDestEnd)
                {
                    *(pDest++) = pixel1;// << bitshift;
                    if(pDest < pDestEnd)
                        *(pDest++) = pixel2;// << bitshift;
                }
                
                pSrc++;
            }
        }
        
        // exposure
        tPvUint32 * data = (tPvUint32*) pvFrame.AncillaryBuffer;
        exposureMicroseconds = Endian32_Swap(data[2]);
        
        // convert to float for display and calculations
        
        vImageConvert_16UToF (&vImage16U, &vImageFloat, 0, K, kvImageNoFlags);
        
        frameNumber = pFrame->FrameCount;
        
        // calculate histogram and brightness
        
        vImagePixelCount vipcHistogram[HISTOGRAM_BINS];
        
        vImageHistogramCalculation_PlanarF ( &vImageFloat,vipcHistogram,HISTOGRAM_BINS,0.0,1.0,kvImageNoFlags);
        
        float brightnessSummed = 0.0;
        
        for(int i = 0; i < HISTOGRAM_BINS; i++){
            histogram[i] = 1.0*vipcHistogram[i]/(width*height);
            brightnessSummed += (i*histogram[i]);
        }
        
        brightness = brightnessSummed / HISTOGRAM_BINS;
        
        // find index of max value
                
        vDSP_maxvi (    histogram,
                    1,
                    &histogramMax,
                    &histogramMaxIndex,
                    HISTOGRAM_BINS
                    );
    }
    
}

void BracketBuffer::updateFrom(BracketBuffer *  buffer)
{
    // copy current frame to former frame
    cblas_scopy (vImageFloat.height*vImageFloat.width,
                 pixelsFloat,1,
                 formerPixelsFloat,1);

    brightness = buffer->brightness;
    histogramMaxIndex = buffer->histogramMaxIndex;
    exposureMicroseconds = buffer->exposureMicroseconds;
    
    for(int i =0; i < HISTOGRAM_BINS; i++){
        histogram[i] = buffer->histogram[i];
    }
    
    frameNumber = buffer->frameNumber;

    
    
        cblas_ccopy (vImageFloat.height*vImageFloat.width/2,
                 buffer->pixels16U,1,
                 pixels16U,1);
    

    cblas_scopy (vImageFloat.height*vImageFloat.width,
                 buffer->pixelsFloat,1,
                 pixelsFloat,1);

    // calculate difference
    
    cblas_saxpy (width*height,-1.0,pixelsFloat,1,formerPixelsFloat,1);
    absDifference = cblas_sasum (width*height,formerPixelsFloat,1)/(width*1.0*height);
        
    // update average frame
    
    float averageAlpha = 1.0/(framesAddedToAverage+1.0);
    
    absDifferenceAverage = (absDifferenceAverage*(1.0-averageAlpha))+(averageAlpha*absDifference);
    
    cblas_sscal(width*height, 1.0-averageAlpha ,averagePixelsFloat, 1);
    cblas_saxpy(width*height, averageAlpha, pixelsFloat, 1, averagePixelsFloat, 1);
    
    vImageConvert_FTo16U(&averageVImageFloat, &averageVImage16U, 0, K, kvImageNoFlags);
        
    // exposure averager
    
    double exposureAverager = (exposureMicrosecondsAverage * (1.0-averageAlpha)) + (exposureMicroseconds * averageAlpha);
    
    exposureMicrosecondsAverage = round(exposureAverager);
    
    framesAddedToAverage++;
    
    needsUpdate = true;
}

void BracketBuffer::update()
{
    if(needsUpdate){    
        texture.update(channel);
        averageTexture.update(averageChannel);
        needsUpdate =false;
    }
}

void BracketBuffer::saveFrame(const char * filename){
    
    fs::path thePath = fs::absolute(fs::path(filename));
    saveBuffer(thePath, &vImage16U);
    
}

void BracketBuffer::saveAverageFrame(const char * filename){
    
    fs::path thePath = fs::absolute(fs::path(filename));
    saveBuffer(thePath, &averageVImage16U);
    
}

void BracketBuffer::saveBuffer(fs::path filePath, vImage_Buffer * vImg){
    
    bool compress = true;
    
    if(filePath.has_parent_path()){
        if (!fs::exists(filePath.parent_path())) {
            fs::create_directory(filePath.parent_path());
            std::cout << "BracketBuffer::saveBuffer CREATEDIR: " << filePath.parent_path() << std::endl;
        }
    }
    
    if(compress){
        filePath = fs::path(str(boost::format("%s.gz") % filePath.c_str()));
    }
    
    using namespace std;
    
    namespace io = boost::iostreams;
    
    stringstream ss(stringstream::in | stringstream::out | stringstream::binary); //Declare ss
    ss.write((char *)vImg->data, vImg->rowBytes*height); //Write data to ss
    
    
    io::filtering_streambuf<io::input> buf; //Declare buf
    if(compress){
        buf.push(io::zlib_compressor(io::zlib_params( io::zlib::best_speed,io::zlib::deflated,15+16,8,io::zlib::default_strategy,false ))); //zlib compression
    }
    buf.push(ss); //Push ss to buf
    ofstream out(filePath.c_str(), ios_base::out | ios_base::binary); //Declare out
    io::copy(buf, out); //Copy buf to out
    
    //Clean up
    out.close();
    
}


BracketBuffer::~BracketBuffer() {
    
    delete pixels16U;
    delete pixelsFloat;
}
