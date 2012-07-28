#ifndef IMAGE_H
#define IMAGE_H 1

// TODO: port to Win32

#include <stdint.h>
#include <cassert>
#include <cstring>

#include <libjp4/misc.h>

template <typename T>
struct PixelRGB {

  static const uint8_t _n_channel = 3;

  union {
    T _channel[_n_channel];
    struct {
      T _R;
      T _G;
      T _B;
    };
  };

  PixelRGB() { _R = _G = _B = T(); }
  PixelRGB(T iR, T iG, T iB) { _R = iR; _G = iG; _B = iB; }
  PixelRGB(const PixelRGB<T>& i) { _R = i._R; _G = i._G; _B = i._B; }

  T& R() { return _R; }
  T& G() { return _G; }
  T& B() { return _B; }

  const T& R() const { return _R; }
  const T& G() const { return _G; }
  const T& B() const { return _B; }

  T& R(T R) { _R = R; return *this; }
  T& G(T G) { _G = G; return *this; }
  T& B(T B) { _B = B; return *this; }

  T& operator[](uint8_t i) { return _channel[i]; }

  PixelRGB<T>& operator=(const PixelRGB<T>& i) { _R = i.R(); _G = i.G(); _B = i.B(); return *this; } 

};

template <typename T>
class Image {
public:
	
	typedef T pixel_type;
	typedef T& pixel_type_ref;
	
  Image() {
    _data = NULL;
    _ysize = 0;
    _xsize = 0;
  }

  Image(uint16_t xsize, uint16_t ysize) {
    _data = new T[ysize*xsize];
    _ysize = ysize;
    _xsize = xsize;
  }

  virtual ~Image() {
		if (_data)
		  delete[] _data;
	}

  uint16_t xsize() const { return _xsize; }
  uint16_t ysize() const { return _ysize; }

  T* data() const { return _data; }

  // raw access
  T& operator[](uint32_t i) const { return _data[i]; }

  // smart access
  T& operator()(uint16_t y) const { return _data[ y*_xsize ]; }
  T& operator()(uint16_t y, uint16_t x) const { return _data[ y*_xsize+x ]; }

  T& at(uint16_t y) const { return operator()(y); }
  T& at(uint16_t y, uint16_t x) const { return operator()(y,x); }

  // transforms
  void flipX() {
    
    for (uint16_t y_src = 0, y_dst=_ysize-1; y_src < _ysize/2; y_src++, y_dst--) {
      for (uint16_t x_src = 0; x_src < _xsize; x_src++) {
        uint32_t src = y_src*_xsize + x_src;
        uint32_t dst = y_dst*_xsize + x_src;
        T tmp = _data[src];
        _data[src] = _data[dst];
        _data[dst] = tmp;
      }
    }
  }

  void flipY() {
    for (uint16_t y_src = 0; y_src < _ysize; y_src++) {
      for (uint16_t x_src = 0, x_dst=_xsize-1; x_src < _xsize/2; x_src++, x_dst--) {
        uint32_t src = y_src*_xsize + x_src;
        uint32_t dst = y_src*_xsize + x_dst;
        T tmp = _data[src];
        _data[src] = _data[dst];
        _data[dst] = tmp;
      }
    }
  }

  void flipXY() {
    flipX(); flipY();
  }

  void rotate(ImageRotation angle) {

    switch (angle) {

    case ROTATE_0:
      break;

    case ROTATE_90: {
      T* _newData = new T[_xsize*_ysize];

      for (uint16_t y_src = 0, y_dst=_ysize; y_src < _ysize; y_src++, y_dst--) {
        for (uint16_t x_src = 0; x_src < _xsize; x_src++) {
          uint32_t src = y_src*_xsize + x_src;
          uint32_t dst = x_src*_ysize + y_dst;
          _newData[dst] = _data[src];
        }
      }

      delete[] _data;
      _data = _newData;

      uint16_t _xsize_tmp = _xsize;
      _xsize = _ysize;
      _ysize = _xsize_tmp;
      break;
    }
     
    case ROTATE_180: {
      flipX(); flipY();
      break;
    }

    case ROTATE_270: {
      T* _newData = new T[_xsize*_ysize];

      for (uint16_t y_src = 0; y_src < _ysize; y_src++) {
        for (uint16_t x_src = 0, x_dst=_xsize-1; x_src < _xsize; x_src++, x_dst--) {
          uint32_t src = y_src*_xsize + x_src;
          uint32_t dst = x_dst*_ysize + y_src;
          _newData[dst] = _data[src];
        }
      }

      delete[] _data;
      _data = _newData;

      uint16_t _xsize_tmp = _xsize;
      _xsize = _ysize;
      _ysize = _xsize_tmp;
      break;
    }

    default:
      break;

    }
  }

  Image<T>* crop(uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize) {
    assert(x < _xsize);
    assert(y < _ysize);
    assert(x+xsize <= _xsize);
    assert(y+ysize <= _ysize);

    Image<T>* cropped = new Image<T>(xsize, ysize);

    for (uint16_t i = 0; i < cropped->ysize(); i++)
      for (uint16_t j = 0; j < cropped->xsize(); j++)
        cropped->at(i,j) = this->at(i+y,j+x);

    return cropped;
  }

private:
  T* _data;

  uint16_t _ysize;
  uint16_t _xsize;
};

typedef PixelRGB<uint8_t>  PixelRGB8i;
typedef PixelRGB<uint16_t> PixelRGB16i;
typedef PixelRGB<uint32_t> PixelRGB32i;
typedef PixelRGB<float>  PixelRGB32f;
typedef PixelRGB<double> PixelRGB64f;

typedef Image<PixelRGB8i>  ImageRGB8i;
typedef Image<PixelRGB16i> ImageRGB16i;
typedef Image<PixelRGB32i> ImageRGB32i;
typedef Image<PixelRGB32f> ImageRGB32f;
typedef Image<PixelRGB64f> ImageRGB64f;

typedef Image<uint8_t>  ImageL8i;
typedef Image<uint16_t> ImageL16i;
typedef Image<uint32_t> ImageL32i;
typedef Image<float>    ImageL32f;
typedef Image<double>   ImageL64f;

#endif
