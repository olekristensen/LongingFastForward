//
//  BracketBuffer.h
//  lffCinderCapture
//
//  Created by Ole Kristensen on 13/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef lffCinderCapture_BracketBuffer_h
#define lffCinderCapture_BracketBuffer_h

#include "cinder/gl/Fbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Channel.h"

namespace fs = boost::filesystem;

class BracketBuffer {
    
public:
    
    BracketBuffer(int _width, int _height, int _bracketNumber);
    
    ~BracketBuffer();

    void load(tPvFrame * pFrame);
    void update();
    void draw();
    void saveFrame(const char * filename);
    void saveAverageFrame(const char * filename);
    void saveBuffer(fs::path filePath, vImage_Buffer * vImg);

    tPvFrame pvFrame;
       
    int width;
    int height;

    unsigned long frameNumber;
    unsigned long framesAddedToAverage;
    int bracketIndex;
    unsigned long exposureMicroseconds;
       
    // latest frame
    
    float * pixelsFloat;
    vImage_Buffer vImageFloat;
    
    USHORT * pixels16U;
    vImage_Buffer vImage16U;
    
    cinder::Channel32f  channel;
    cinder::gl::Texture texture;

    // former frame
    
    USHORT * formerPixels16U;
    vImage_Buffer formerVImage16U;
    
    float absDifference; 
        
    // average frame
    
    float * averagePixelsFloat;
    vImage_Buffer averageVImageFloat;
    
    USHORT * averagePixels16U;
    vImage_Buffer averageVImage16U;
    
    cinder::Channel32f  averageChannel;
    cinder::gl::Texture averageTexture;
    
    bool needsUpdate;
    
    
};

#endif
