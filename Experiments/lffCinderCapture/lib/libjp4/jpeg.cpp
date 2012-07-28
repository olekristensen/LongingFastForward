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
#include <jpeglib.h>
}


void JP4::writeJPEG(const string& jpegFilename, unsigned int quality) const {

  struct jpeg_error_mgr jerr;
  struct jpeg_compress_struct cinfo;

  FILE *ofp;

  cinfo.err = jpeg_std_error (&jerr);

  ofp = fopen(jpegFilename.c_str(), "wb");

  jpeg_create_compress (&cinfo);
  jpeg_stdio_dest (&cinfo, ofp);

  cinfo.image_width = width();
  cinfo.image_height = height();
  cinfo.input_components = 1;
  cinfo.in_color_space = JCS_GRAYSCALE;
  jpeg_set_defaults(&cinfo);

  jpeg_set_quality(&cinfo, quality, false);

  jpeg_start_compress (&cinfo, TRUE);

  // write EXIF data (if any)
  if (_raw_app1) {
    //jpeg_write_marker(&cinfo, 0xe1, _raw_app1, _raw_app1_length);
  }

  unsigned char* scanline = new unsigned char[width()];

  for (unsigned int i=0; i < height(); i++) {
    for (unsigned int j=0; j < width(); j++)
      scanline[j] = _image->at(i,j);

    jpeg_write_scanlines (&cinfo, &scanline, 1);
  }

  jpeg_finish_compress (&cinfo);
  jpeg_destroy_compress (&cinfo);

  fclose(ofp);

  delete[] scanline;
}

