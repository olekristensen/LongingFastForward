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

class BracketBuffer {
    
public:
    
    BracketBuffer(int _width, int _height, int _bracketNumber);
    
    ~BracketBuffer();

    void update(tPvFrame * pFrame);
    void saveFrame(char * filename);
    
    tPvFrame pvFrame;
    
    float * pixelsFloat;
    USHORT * pixels16U;
    
    vImage_Buffer vImageFloat;
    vImage_Buffer vImage16U;
    
    unsigned long frameNumber;
    int bracketIndex;
    unsigned long exposureMicroseconds;
    cinder::gl::Texture texture;
    cinder::gl::Fbo     fbo;
    
    int width;
    int height;
    
    
};

#endif
