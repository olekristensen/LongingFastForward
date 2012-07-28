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

#ifndef JP4_H
#define JP4_H 1

#include <libjp4/image.h>
#include <libjp4/bayer.h>

#include <string>
using std::string;

#include <memory>
using std::auto_ptr;

#include <vector>
using std::vector;

extern "C" {
#include <libexif/exif-data.h>
#include <libexif/exif-tag.h>
}

typedef struct {
  double gain[4];
  double gamma[4];
  double gamma_scale[4];
  double black[4];
  int    woi_left;
  int    woi_width;
  int    woi_top;
  int    woi_height;
  bool   flip_hor;
  bool   flip_ver;
  int    bayer_mode;
  int    color_mode;
  int    decim_hor;
  int    decim_ver;
  int    bin_hor;
  int    bin_ver;
  bool   composite;
  int    height1;
  int    blank1;
  bool   flip_h1;
  bool   flip_v1;
  int    height2;
  int    blank2;
  bool   flip_h2;
  bool   flip_v2;
  int    height3;
  bool   flip_h3;
  bool   flip_v3;
  bool   portrait;
} ElphelMakerNote;

typedef struct {
  int x;
  int y;
  int width;
  int height;
} ROI;

class JP4 {

 public:

  JP4();
  ~JP4();

  bool open(const string& _filename);

  const string& filename() const;

  unsigned int width() const;
  unsigned int height() const;

  unsigned short* data() const;

  ImageL16i* image() const;

  Bayer* bayer();

  const ElphelMakerNote& makerNote() const;

  ElphelMakerNote& makerNote();

  unsigned int makerNoteLength() const;

  bool linear() const;

  JP4* crop(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

  void writePGM(const string& pgmFilename) const;

  void writeJPEG(const string& jpegFilename, unsigned int quality) const;

  void writeDNG(const string& dngFilename, int bayerShift = -1) const;

  void demux(vector<JP4*>& vec);

  //
  // EXIF support
  //
  bool hasTag(ExifTag tag) const;

  unsigned int getTagUInt(ExifTag tag) const;

  string getTagString(ExifTag tag) const;

  void getTagURational(ExifTag tag, unsigned int* n, unsigned int* d) const;

  void getTagSRational(ExifTag tag, int* n, int* d) const;

  ExifEntry* getTagRaw(ExifTag tag) const;

  ExifData* exifData() const;

 private:
  JP4(const JP4& other); // non-conpyable
  JP4& operator=(const JP4& other); // non-assignable

  void readMakerNote();

  string _filename;

  ImageL16i* _image;
  Bayer      _bayer;

  ElphelMakerNote _makerNote;
  unsigned int    _makerNoteLength;

  bool _linear;

  unsigned char* _raw_app1;
  unsigned short _raw_app1_length;
  ExifData* _ed;

};

#endif
