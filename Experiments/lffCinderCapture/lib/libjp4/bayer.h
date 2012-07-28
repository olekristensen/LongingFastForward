#ifndef JP4_BAYER_H
#define JP4_BAYER_H 1

#include <libjp4/misc.h>
#include <stdint.h>

typedef enum {
  /*
   *  +-+-+  +-+-+  +-+-+  +-+-+
   *  |G|R|  |R|G|  |B|G|  |G|B|
   *  +-+-+  +-+-+  +-+-+  +-+-+
   *  |B|G|  |G|B|  |G|R|  |R|G|
   *  +-+-+  +-+-+  +-+-+  +-+-+
   */
      GR_BG, RG_GB, BG_GR, GB_RG

} BayerMatrix;

static BayerMatrix _lut_flipX[4]     = {BG_GR, GB_RG, GR_BG, RG_GB};
static BayerMatrix _lut_flipY[4]     = {RG_GB, GR_BG, GB_RG, BG_GR};
static BayerMatrix _lut_rotate90[4]  = {BG_GR, GR_BG, GB_RG, RG_GB};
static BayerMatrix _lut_rotate270[4] = {RG_GB, GB_RG, GR_BG, BG_GR};

class Bayer {

 public:

  Bayer(BayerMatrix bayer = GR_BG): _bayer(bayer) {}

  void flipX() { _bayer = _lut_flipX[_bayer]; }
  void flipY() { _bayer = _lut_flipY[_bayer]; }
  void flipXY() { flipX(); flipY(); }

  void rotate(ImageRotation rotation) {
    switch (rotation) {
    case ROTATE_0:
      break;
    case ROTATE_90:
      _bayer = _lut_rotate90[_bayer];
      break;
    case ROTATE_180:
      flipXY();
      break;
    case ROTATE_270:
      _bayer = _lut_rotate270[_bayer];
      break;
    default:
      break;
    }
  }

  int8_t value() const { return _bayer; }

 private:

  int8_t _bayer;

};

#endif
