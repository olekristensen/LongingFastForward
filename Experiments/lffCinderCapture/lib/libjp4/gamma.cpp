/*
  Copyright 2010 Paulo Henrique Silva <ph.silva@gmail.com>

  This file is part of jp4-tools.

  jp4-tools is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  jp4-tools is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with jp4-tools.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "gamma.h"

#include <cmath>

void Gamma::reverseTable(double gamma, double gamma_scale, double black, unsigned short* rgTable) {

  int i;
  double x, black256 ,k;
  int* gtable = new int[257];
  int ig;

  black256=black*256.0;
  
  k = 1.0/(256.0-black256);

  if (gamma < 0.13) gamma=0.13;
  if (gamma >10.0)  gamma=10.0;

  for (i=0; i<257; i++) {
    x=k*(i-black256);
    if (x < 0.0 ) x=0.0;
    ig= (int) (0.5+65535.0*pow(x,gamma));
    ig=(ig* (int) gamma_scale)/0x400;
    if (ig > 0xffff) ig=0xffff;
    gtable[i]=ig;
  }

  /** now gtable[] is the same as was used in the camera */
  /** FPGA was using linear interpolation between elements of the gamma table, so now we'll reverse that process */
  int indx=0;
  unsigned short outValue;
  
  for (i=0; i<256; i++ ) {
    outValue=128+(i<<8);
    
    while ((gtable[indx+1]<outValue) && (indx<256)) indx++;
    
    if (indx>=256)
      rgTable[i]=(65535.0/256);
    else if (gtable[indx+1]==gtable[indx])
      rgTable[i]=i;
    else
      rgTable[i]=256.0*(indx+(1.0*(outValue-gtable[indx]))/(gtable[indx+1] - gtable[indx]));
  }

  delete[] gtable;
}
