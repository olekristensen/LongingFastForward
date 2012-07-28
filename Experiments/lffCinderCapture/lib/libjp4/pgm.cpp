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

#include <libjp4/jp4.h>

extern "C" {
#include <stdio.h>
}


void JP4::writePGM(const string& pgmFilename) const {

  FILE* pgm = fopen(pgmFilename.c_str(), "w");
  if (!pgm) return;

  fprintf(pgm, "P2\n%d %d\n%d\n", width(), height(), 0xff);
  for (unsigned int i = 0; i < height(); i++) {
    for (unsigned int j = 0; j < width(); j++) {
      fprintf(pgm, "%d ", _image->at(i,j));
    }   
    fprintf(pgm, "\n");
  }

  fclose(pgm);

}

