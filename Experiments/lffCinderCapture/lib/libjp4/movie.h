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

#ifndef MOVIE_H
#define MOVIE_H 1

#include <string>
using std::string;

extern "C" {
#include <libavformat/avformat.h>
}

class Movie;

class MovieIterator {

 public:
  MovieIterator(Movie* movie, unsigned int start = 0, unsigned int end = 0);
  virtual ~MovieIterator();

  bool hasNext() const;
  bool next(unsigned int* frame, void** data, unsigned int* size);

 private:
  Movie* movie;
  int start;
  int end;

  int currentFrame;
  AVPacket packet;
};

class Movie {

 public:

  Movie();
  virtual ~Movie();

  bool open(const string& filename);

  unsigned int  nFrames() const;
  MovieIterator iterator(unsigned int start = -1, unsigned int end = -1);

  friend class MovieIterator;

 private:

  AVFormatContext* ctx;

};

#endif // !MOVIE_H
