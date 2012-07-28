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

#include "movie.h"

MovieIterator::MovieIterator(Movie* movie, unsigned int start, unsigned int end)
  : movie(movie), start(start), end(end), currentFrame(0)
{
  if (start == 0) start = 1;
  if (end == 0 || end > movie->nFrames()) end = movie->nFrames();
}

MovieIterator::~MovieIterator() {
  av_free_packet(&packet);
}

bool MovieIterator::hasNext() const {
  return currentFrame < end;
}

bool MovieIterator::next(unsigned int* frame, void** data, unsigned int* size) {
  if (currentFrame >= end)
    return false;

  if (currentFrame > 0)
    av_free_packet(&packet);

  int res = av_read_frame(movie->ctx, &packet);
  if (res >= 0) {
    if (frame) *frame = currentFrame+1;
    if (data) *data = packet.data;
    if (size) *size = packet.size;
    currentFrame++;
    return true;
  }

  return false;
}

Movie::Movie(): ctx(NULL) {
}

bool Movie::open(const string& filename) {

  av_register_all();

  if (av_open_input_file(&ctx, filename.c_str(), NULL, 0, NULL) != 0) {
    fprintf(stderr, "ERROR: Cannot open file: '%s'.\n", filename.c_str());
    return false;
  }

  if (av_find_stream_info(ctx) < 0) {
    fprintf(stderr, "ERROR: Cannot find stream info.\n");
    return false;
  }
  
  AVCodecContext* codecCtx = ctx->streams[0]->codec;
  AVCodec* codec = avcodec_find_decoder(codecCtx->codec_id);
  if (!codec) {
    fprintf(stderr, "ERROR: Cannot find codec.");
    return false;
  }
  
  if (avcodec_open(codecCtx, codec) < 0) {
    fprintf(stderr, "ERROR: Cannot open codec.");
    return false;
  }

  return true;
}

Movie::~Movie() {
}

unsigned int Movie::nFrames() const {
  return ctx->streams[0]->nb_frames;
}

MovieIterator Movie::iterator(unsigned int start, unsigned int end) {
  return MovieIterator(this, start, end);
}
