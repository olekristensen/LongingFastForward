//
//  BracketBuffer.cpp
//  lffCinderCapture
//
//  Created by Ole Kristensen on 13/07/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "BracketBuffer.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Texture.h"
#include <iostream>


BracketBuffer::BracketBuffer(int _width, int _height, int _bracketIndex){
    
    frameNumber = 0;
    
    bracketIndex = _bracketIndex;
    width = _width;
    height = _height;
    
    pixelsFloat = new float [width * height];
    memset(pixelsFloat, 0.0, width*height);
    vImageFloat.data = pixelsFloat;
    vImageFloat.width = width;
    vImageFloat.height = height;
    vImageFloat.rowBytes = width*4;
    
    pixels16U = new USHORT [width * height];
    memset(pixels16U, 0xFFFF, width*height);
    vImage16U.data = pixelsFloat;
    vImage16U.width = width;
    vImage16U.height = height;
    vImage16U.rowBytes = width*4;
    
    
}

void BracketBuffer::update(tPvFrame * pFrame)
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
        
        
        pDest = (USHORT*)pvFrame.ImageBuffer;
        pDestEnd = pDest + (pFrame->Width * pFrame->Height);
        
        // convert to float for display
        
        const float K = 1.0 / 0xFFF;
        
        vImageConvert_16UToF (&vImage16U,
                              &vImageFloat,
                              0,
                              K,
                              kvImageNoFlags);
    }
    
}

void BracketBuffer::saveFrame(char * filename){
    
    FILE * pFile;
    pFile = std::fopen (filename,"w");
    if (pFile!=NULL)
    {
        fwrite(pvFrame.ImageBuffer, pvFrame.ImageBufferSize, 1, pFile);
        fclose (pFile);
    } else {
        std::cout << "FAILED: BracketBuffer::saveFrame (" << filename << ")" << std::endl;
    }
    
    delete filename;
    
}

BracketBuffer::~BracketBuffer() {
    delete pixels16U;
    delete pixelsFloat;
}
