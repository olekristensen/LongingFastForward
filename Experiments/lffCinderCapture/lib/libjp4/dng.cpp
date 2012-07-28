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

#include "dng_camera_profile.h"
#include "dng_color_space.h"
#include "dng_exceptions.h"
#include "dng_file_stream.h"
#include "dng_globals.h"
#include "dng_host.h"
#include "dng_ifd.h"
#include "dng_image_writer.h"
#include "dng_info.h"
#include "dng_linearization_info.h"
#include "dng_memory_stream.h"
#include "dng_mosaic_info.h"
#include "dng_negative.h"
#include "dng_preview.h"
#include "dng_rational.h"
#include "dng_read_image.h"
#include "dng_render.h"
#include "dng_simple_image.h"
#include "dng_tag_codes.h"
#include "dng_tag_types.h"
#include "dng_tag_values.h"
#include "dng_xmp.h"
#include "dng_xmp_sdk.h"

#include <libjp4/jp4.h>
#include <libjp4/gamma.h>

#include <vector>
using std::vector;

inline void SET_DNG_TAG_STRING(const JP4& jp4, ExifTag TAG, dng_string* DEST) {
  if (jp4.hasTag(TAG)) {                     
    dng_string s;                            
    s.Set(jp4.getTagString(TAG).c_str());    
    *DEST = s;                          
  }                                          
}

inline void SET_DNG_TAG_UINT32(const JP4& jp4, ExifTag TAG, uint32* DEST) {
  if (jp4.hasTag(TAG)) {                     
    *DEST = jp4.getTagUInt(TAG);        
  }                                     
}

inline void SET_DNG_TAG_URATIONAL(const JP4& jp4, ExifTag TAG, dng_urational* DEST) {
  if (jp4.hasTag(TAG)) {                     
    unsigned int n = 0; unsigned int d = 1;                    
    jp4.getTagURational(TAG, &n, &d);         
    *DEST = dng_urational(n,d);          
  }                                          
}

inline void SET_DNG_TAG_SRATIONAL(const JP4& jp4, ExifTag TAG, dng_srational* DEST) {
  if (jp4.hasTag(TAG)) {                     
    int n = 0; int d = 1;                    
    jp4.getTagSRational(TAG, &n, &d);         
    *DEST = dng_srational(n,d);          
  }                                          
}

inline void SET_DNG_TAG_DT_INFO(const JP4& jp4, ExifTag TAG, ExifTag SUBSEC_TAG, dng_date_time_info* DEST) {
  if (jp4.hasTag(TAG)) {                     
    string s = jp4.getTagString(TAG);        
    dng_date_time dt;                        
    dt.Parse(s.c_str());           

    dng_date_time_info dt_info;              
    dt_info.SetDateTime(dt);                

    if (jp4.hasTag(SUBSEC_TAG)) {
      string subsec = jp4.getTagString(SUBSEC_TAG);        
      dng_string dngSubsec;
      dngSubsec.Set(subsec.c_str());
      dt_info.SetSubseconds(dngSubsec);
    }

    *DEST = dt_info;
  }                                          
} 

ExifEntry* FIND_GPS_TAG(const JP4& jp4, int TAG) {
  ExifData* ed = jp4.exifData();
  ExifContent* gps = ed->ifd[EXIF_IFD_GPS];
  if (!gps) return NULL;

  // find given tag withing entries
  ExifEntry* e = NULL;
  for (unsigned int i = 0; i < gps->count; i++)
    if (gps->entries[i]->tag == TAG)
      e = gps->entries[i];

  return e;
}

inline void SET_DNG_TAG_GPS_UINT32(const JP4& jp4, int TAG, uint32* DEST) {
  ExifEntry* e = FIND_GPS_TAG(jp4, TAG);
  if (e==NULL)
    return;
  *DEST = exif_get_long(e->data, exif_data_get_byte_order(jp4.exifData()));
}

inline void SET_DNG_TAG_GPS_STRING(const JP4& jp4, int TAG, dng_string* DEST) {
  ExifEntry* e = FIND_GPS_TAG(jp4, TAG);
  if (e==NULL)
    return;

  char value[e->size];
  exif_entry_get_value(e, value, e->size);

  dng_string s;                            
  s.Set(value);    
  *DEST = s;                          
}


inline void SET_DNG_TAG_GPS_URATIONAL(const JP4& jp4, int TAG, dng_urational* DEST) {
  ExifEntry* e = FIND_GPS_TAG(jp4, TAG);
  if (e==NULL)
    return;
  ExifRational r = exif_get_rational(e->data, exif_data_get_byte_order(jp4.exifData()));
  dng_urational dngR(r.numerator, r.denominator);
  *DEST = dngR;
}

inline void SET_DNG_TAG_GPS_URATIONAL_ARRAY_3(const JP4& jp4, int TAG, dng_urational(*DEST)[3]) {
  ExifEntry* e = FIND_GPS_TAG(jp4, TAG);
  if (e==NULL)
    return;

  if (e->components != 3) return;

  ExifRational r1 = exif_get_rational(e->data, exif_data_get_byte_order(jp4.exifData()));
  dng_urational dngR1(r1.numerator, r1.denominator);
  (*DEST)[0] = dngR1;

  ExifRational r2 = exif_get_rational(e->data+exif_format_get_size(e->format), exif_data_get_byte_order(jp4.exifData()));
  dng_urational dngR2(r2.numerator, r2.denominator);
  (*DEST)[1] = dngR2;

  ExifRational r3 = exif_get_rational(e->data+2*exif_format_get_size(e->format), exif_data_get_byte_order(jp4.exifData()));
  dng_urational dngR3(r3.numerator, r3.denominator);
  (*DEST)[2] = dngR3;
}    

void JP4::writeDNG(const string& dngFilename, int bayerShift) const {

  // TODO
  unsigned int whitePoint = 0xffff;
  
  // DNG memory allocation and initialization

  dng_memory_allocator memalloc(gDefaultDNGMemoryAllocator);
  dng_memory_stream stream(memalloc);
  stream.Put(data(), width()*height()*sizeof(unsigned short));

  dng_rect rect(height(), width());

  dng_host host(&memalloc);
  host.SetSaveDNGVersion(dngVersion_SaveDefault);
  host.SetSaveLinearDNG(false);
  host.SetKeepOriginalFile(true);

  AutoPtr<dng_image> image(new dng_simple_image(rect, 1, ttShort, memalloc));

  // DNG IFD structure creation

  dng_ifd ifd;

  ifd.fUsesNewSubFileType        = true;
  ifd.fCompression               = ccUncompressed;

  ifd.fPlanarConfiguration       = pcPlanar;
  ifd.fPhotometricInterpretation = piCFA;

  ifd.fImageWidth                = width();
  ifd.fImageLength               = height();

  ifd.fTileWidth                 = width();
  ifd.fTileLength                = height();

  ifd.fSamplesPerPixel           = 1;
  ifd.fBitsPerSample[0]          = 16;
  ifd.fSampleFormat[0]           = sfUnsignedInteger;

  ifd.fBlackLevel[0][0][0]       = makerNote().black[0]*256;
  ifd.fBlackLevel[0][0][1]       = makerNote().black[1]*256;
  ifd.fBlackLevel[0][0][2]       = makerNote().black[2]*256;
  ifd.fBlackLevel[0][0][3]       = makerNote().black[3]*256;

  ifd.fWhiteLevel[0]             = whitePoint;
  
  ifd.fLinearizationTableType   = ttShort;
  ifd.fLinearizationTableCount  = 256;

  ifd.fCFARepeatPatternRows      = 2;
  ifd.fCFARepeatPatternCols      = 2;

  ifd.fActiveArea                = rect;
  ifd.fDefaultCropSizeH          = dng_urational(width(), 1);
  ifd.fDefaultCropSizeV          = dng_urational(height(), 1);

  ifd.ReadImage(host, stream, *image.Get());

  // DNG Negative structure creation

  AutoPtr<dng_negative> negative(host.Make_dng_negative());

  negative->SetDefaultScale(ifd.fDefaultScaleH, ifd.fDefaultScaleV);
  negative->SetDefaultCropOrigin(ifd.fDefaultCropOriginH, ifd.fDefaultCropOriginV);
  negative->SetDefaultCropSize(ifd.fDefaultCropSizeH, ifd.fDefaultCropSizeV);
  negative->SetActiveArea(ifd.fActiveArea);

  negative->SetOriginalRawFileName(filename().c_str());

  negative->SetWhiteLevel(whitePoint, 0);

  negative->SetBlackLevel(makerNote().black[0]*65536, 0);
  negative->SetBlackLevel(makerNote().black[1]*65536, 1);
  negative->SetBlackLevel(makerNote().black[2]*65536, 2);
  negative->SetBlackLevel(makerNote().black[3]*65536, 3);

  // linearization table (handles gamma, gamma_scale and black level)
  AutoPtr<dng_memory_block> curve(memalloc.Allocate(256*sizeof(unsigned short)));
  Gamma::reverseTable(makerNote().gamma[0],
                      makerNote().gamma_scale[0],
                      makerNote().black[0],
                      curve->Buffer_uint16());
  negative->SetLinearization(curve);

  // gain
  dng_vector gain = dng_vector(4);
  gain[0] = makerNote().gain[0];
  gain[1] = makerNote().gain[1];
  gain[2] = makerNote().gain[2];
  gain[3] = makerNote().gain[3];

  negative->SetAnalogBalance(gain);
  
  // bayer
  negative->SetRGB();
  
  bool flip_hor = makerNote().flip_hor;
  bool flip_ver = makerNote().flip_ver;
  
  // see http://www.mozoft.com/tifftest/ContactSheet-001.gif for Orientation hints
  // G R
  // B G
  if (flip_hor == 0 && flip_ver == 0) {
    negative->SetBayerMosaic(0);
    negative->SetBaseOrientation(dng_orientation::Normal());
    // R G
    // G B
  } else if (flip_hor == 1 && flip_ver == 0) {
    negative->SetBayerMosaic(1);
    negative->SetBaseOrientation(dng_orientation::Mirror());
    // B G
    // G R
  } else if (flip_hor == 0 && flip_ver == 1) {
    negative->SetBayerMosaic(2);
    negative->SetBaseOrientation(dng_orientation::Mirror180());
    // G B
    // R G
  } else if (flip_hor == 1 && flip_ver == 1) {
    negative->SetBayerMosaic(3);
    negative->SetBaseOrientation(dng_orientation::Rotate180());
  }
  
  // Override bayer shift if asked
  if (bayerShift != -1)
    negative->SetBayerMosaic(bayerShift);
  
  // -------------------------------------------------------------------------------

  // Set Camera->XYZ Color matrix as profile.
  dng_matrix_3by3 matrix(2.005, -0.771, -0.269, -0.752, 1.688, 0.064, -0.149, 0.283, 0.745);

  AutoPtr<dng_camera_profile> prof(new dng_camera_profile);
  prof->SetColorMatrix1((dng_matrix) matrix);
  prof->SetCalibrationIlluminant1(lsD65);
  negative->AddProfile(prof);

  double cameraMult[] = { 0.807133, 1.0, 0.913289};
  negative->SetCameraNeutral(dng_vector_3(cameraMult[0],
                                          cameraMult[1],
                                          cameraMult[2]));

  // Updating metadata to DNG Negative
  dng_exif *exif = negative->GetExif();

  if (hasTag(EXIF_TAG_MAKER_NOTE)) {
    ExifEntry* mNote = getTagRaw(EXIF_TAG_MAKER_NOTE);
    AutoPtr<dng_memory_block> makerNote(memalloc.Allocate(sizeof(mNote->data)));
    memcpy(makerNote->Buffer(), mNote->data, mNote->size);
    negative->SetMakerNoteSafety(true);
    negative->SetMakerNote(makerNote);
  }

  SET_DNG_TAG_DT_INFO (*this, EXIF_TAG_DATE_TIME,           EXIF_TAG_SUB_SEC_TIME,           &exif->fDateTime);
  SET_DNG_TAG_DT_INFO (*this, EXIF_TAG_DATE_TIME_ORIGINAL,  EXIF_TAG_SUB_SEC_TIME_ORIGINAL,  &exif->fDateTimeOriginal);
  SET_DNG_TAG_DT_INFO (*this, EXIF_TAG_DATE_TIME_DIGITIZED, EXIF_TAG_SUB_SEC_TIME_DIGITIZED, &exif->fDateTimeDigitized);
		
  SET_DNG_TAG_STRING    (*this, EXIF_TAG_IMAGE_DESCRIPTION,         &exif->fImageDescription);
  SET_DNG_TAG_STRING    (*this, EXIF_TAG_MAKE,                      &exif->fMake);
  SET_DNG_TAG_STRING    (*this, EXIF_TAG_MODEL,                     &exif->fModel);
  SET_DNG_TAG_STRING    (*this, EXIF_TAG_SOFTWARE,                  &exif->fSoftware);
  SET_DNG_TAG_STRING    (*this, EXIF_TAG_ARTIST,                    &exif->fArtist);
  SET_DNG_TAG_STRING    (*this, EXIF_TAG_COPYRIGHT,                 &exif->fCopyright);
  SET_DNG_TAG_STRING    (*this, EXIF_TAG_USER_COMMENT,              &exif->fUserComment);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_TIFF_EP_STANDARD_ID,       &exif->fTIFF_EP_StandardID);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_EXIF_VERSION,              &exif->fExifVersion);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_FLASH_PIX_VERSION,         &exif->fFlashPixVersion);
  SET_DNG_TAG_URATIONAL (*this, EXIF_TAG_EXPOSURE_TIME,             &exif->fExposureTime);
  SET_DNG_TAG_URATIONAL (*this, EXIF_TAG_FNUMBER,                   &exif->fFNumber);
  SET_DNG_TAG_SRATIONAL (*this, EXIF_TAG_SHUTTER_SPEED_VALUE,       &exif->fShutterSpeedValue);
  SET_DNG_TAG_URATIONAL (*this, EXIF_TAG_APERTURE_VALUE,            &exif->fApertureValue);
  SET_DNG_TAG_SRATIONAL (*this, EXIF_TAG_BRIGHTNESS_VALUE,          &exif->fBrightnessValue);
  SET_DNG_TAG_SRATIONAL (*this, EXIF_TAG_EXPOSURE_BIAS_VALUE,       &exif->fExposureBiasValue);
  SET_DNG_TAG_URATIONAL (*this, EXIF_TAG_MAX_APERTURE_VALUE,        &exif->fMaxApertureValue);
  SET_DNG_TAG_URATIONAL (*this, EXIF_TAG_FOCAL_LENGTH,              &exif->fFocalLength);
  SET_DNG_TAG_URATIONAL (*this, EXIF_TAG_DIGITAL_ZOOM_RATIO,        &exif->fDigitalZoomRatio);
  SET_DNG_TAG_URATIONAL (*this, EXIF_TAG_EXPOSURE_INDEX,            &exif->fExposureIndex);
  SET_DNG_TAG_URATIONAL (*this, EXIF_TAG_SUBJECT_DISTANCE,          &exif->fSubjectDistance);
  SET_DNG_TAG_URATIONAL (*this, EXIF_TAG_GAMMA,                     &exif->fGamma);
  SET_DNG_TAG_URATIONAL (*this, EXIF_TAG_BATTERY_LEVEL,             &exif->fBatteryLevelR);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_EXPOSURE_PROGRAM,          &exif->fExposureProgram);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_METERING_MODE,             &exif->fMeteringMode);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_LIGHT_SOURCE,              &exif->fLightSource);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_FLASH,                     &exif->fFlash);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_SENSING_METHOD,            &exif->fSensingMethod);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_COLOR_SPACE,               &exif->fColorSpace);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_FILE_SOURCE,               &exif->fFileSource);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_SCENE_TYPE,                &exif->fSceneType);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_CUSTOM_RENDERED,           &exif->fCustomRendered);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_EXPOSURE_MODE,             &exif->fExposureMode);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_WHITE_BALANCE,             &exif->fWhiteBalance);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_SCENE_CAPTURE_TYPE,        &exif->fSceneCaptureType);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_GAIN_CONTROL,              &exif->fGainControl);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_CONTRAST,                  &exif->fContrast);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_SATURATION,                &exif->fSaturation);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_SHARPNESS,                 &exif->fSharpness);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_SUBJECT_DISTANCE_RANGE,    &exif->fSubjectDistanceRange);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_FOCAL_LENGTH_IN_35MM_FILM, &exif->fFocalLengthIn35mmFilm);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_ISO_SPEED_RATINGS,         &exif->fISOSpeedRatings[0]);
  
  if (hasTag(EXIF_TAG_SUBJECT_AREA)) {
    exif->fSubjectAreaCount = 1;
    exif->fSubjectArea[0] = getTagUInt(EXIF_TAG_SUBJECT_AREA);
  }
  
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_COMPONENTS_CONFIGURATION,    &exif->fComponentsConfiguration);
  SET_DNG_TAG_URATIONAL (*this, EXIF_TAG_COMPRESSED_BITS_PER_PIXEL,   &exif->fCompresssedBitsPerPixel);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_PIXEL_X_DIMENSION,           &exif->fPixelXDimension);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_PIXEL_Y_DIMENSION,           &exif->fPixelYDimension);
  SET_DNG_TAG_URATIONAL (*this, EXIF_TAG_FOCAL_PLANE_X_RESOLUTION,    &exif->fFocalPlaneXResolution);
  SET_DNG_TAG_URATIONAL (*this, EXIF_TAG_FOCAL_PLANE_Y_RESOLUTION,    &exif->fFocalPlaneYResolution);
  SET_DNG_TAG_UINT32    (*this, EXIF_TAG_FOCAL_PLANE_RESOLUTION_UNIT, &exif->fFocalPlaneResolutionUnit);

  SET_DNG_TAG_GPS_URATIONAL_ARRAY_3 (*this, EXIF_TAG_GPS_LATITUDE,       &exif->fGPSLatitude);
  SET_DNG_TAG_GPS_URATIONAL_ARRAY_3 (*this, EXIF_TAG_GPS_LONGITUDE,      &exif->fGPSLongitude);		
  SET_DNG_TAG_GPS_URATIONAL_ARRAY_3 (*this, EXIF_TAG_GPS_TIME_STAMP,     &exif->fGPSTimeStamp);
  SET_DNG_TAG_GPS_URATIONAL_ARRAY_3 (*this, EXIF_TAG_GPS_DEST_LATITUDE,  &exif->fGPSDestLatitude);
  SET_DNG_TAG_GPS_URATIONAL_ARRAY_3 (*this, EXIF_TAG_GPS_DEST_LONGITUDE, &exif->fGPSDestLongitude);

  SET_DNG_TAG_GPS_UINT32    (*this, EXIF_TAG_GPS_VERSION_ID,         &exif->fGPSVersionID);
  SET_DNG_TAG_GPS_STRING    (*this, EXIF_TAG_GPS_LATITUDE_REF,       &exif->fGPSLatitudeRef);
  SET_DNG_TAG_GPS_STRING    (*this, EXIF_TAG_GPS_LONGITUDE_REF,      &exif->fGPSLongitudeRef);
  SET_DNG_TAG_GPS_UINT32    (*this, EXIF_TAG_GPS_ALTITUDE_REF,       &exif->fGPSAltitudeRef);
  SET_DNG_TAG_GPS_URATIONAL (*this, EXIF_TAG_GPS_ALTITUDE,           &exif->fGPSAltitude);
  SET_DNG_TAG_GPS_STRING    (*this, EXIF_TAG_GPS_SATELLITES,         &exif->fGPSSatellites);
  SET_DNG_TAG_GPS_STRING    (*this, EXIF_TAG_GPS_STATUS,             &exif->fGPSStatus);
  SET_DNG_TAG_GPS_STRING    (*this, EXIF_TAG_GPS_MEASURE_MODE,       &exif->fGPSMeasureMode);
  SET_DNG_TAG_GPS_URATIONAL (*this, EXIF_TAG_GPS_DOP,                &exif->fGPSDOP);
  SET_DNG_TAG_GPS_STRING    (*this, EXIF_TAG_GPS_SPEED_REF,          &exif->fGPSSpeedRef);
  SET_DNG_TAG_GPS_URATIONAL (*this, EXIF_TAG_GPS_SPEED,              &exif->fGPSSpeed);
  SET_DNG_TAG_GPS_STRING    (*this, EXIF_TAG_GPS_TRACK_REF,          &exif->fGPSTrackRef);
  SET_DNG_TAG_GPS_URATIONAL (*this, EXIF_TAG_GPS_TRACK,              &exif->fGPSTrack);
  SET_DNG_TAG_GPS_STRING    (*this, EXIF_TAG_GPS_IMG_DIRECTION_REF,  &exif->fGPSImgDirectionRef);
  SET_DNG_TAG_GPS_URATIONAL (*this, EXIF_TAG_GPS_IMG_DIRECTION,      &exif->fGPSImgDirection);
  SET_DNG_TAG_GPS_STRING    (*this, EXIF_TAG_GPS_MAP_DATUM,          &exif->fGPSMapDatum);
  SET_DNG_TAG_GPS_STRING    (*this, EXIF_TAG_GPS_DEST_LATITUDE_REF,  &exif->fGPSDestLatitudeRef);
  SET_DNG_TAG_GPS_STRING    (*this, EXIF_TAG_GPS_DEST_LONGITUDE_REF, &exif->fGPSDestLongitudeRef);
  SET_DNG_TAG_GPS_STRING    (*this, EXIF_TAG_GPS_DEST_BEARING_REF,   &exif->fGPSDestBearingRef);
  SET_DNG_TAG_GPS_URATIONAL (*this, EXIF_TAG_GPS_DEST_BEARING,       &exif->fGPSDestBearing);
  SET_DNG_TAG_GPS_STRING    (*this, EXIF_TAG_GPS_DEST_DISTANCE_REF,  &exif->fGPSDestDistanceRef);
  SET_DNG_TAG_GPS_URATIONAL (*this, EXIF_TAG_GPS_DEST_DISTANCE,      &exif->fGPSDestDistance);
  SET_DNG_TAG_GPS_STRING    (*this, EXIF_TAG_GPS_PROCESSING_METHOD,  &exif->fGPSProcessingMethod);
  SET_DNG_TAG_GPS_STRING    (*this, EXIF_TAG_GPS_AREA_INFORMATION,   &exif->fGPSAreaInformation);
  SET_DNG_TAG_GPS_STRING    (*this, EXIF_TAG_GPS_DATE_STAMP,         &exif->fGPSDateStamp);
  SET_DNG_TAG_GPS_UINT32    (*this, EXIF_TAG_GPS_DIFFERENTIAL,       &exif->fGPSDifferential);
		
  SET_DNG_TAG_STRING (*this, EXIF_TAG_INTEROPERABILITY_INDEX,    &exif->fInteroperabilityIndex);
  SET_DNG_TAG_UINT32 (*this, EXIF_TAG_INTEROPERABILITY_VERSION,  &exif->fInteroperabilityVersion);

  SET_DNG_TAG_STRING (*this, EXIF_TAG_RELATED_IMAGE_FILE_FORMAT, &exif->fRelatedImageFileFormat);
  SET_DNG_TAG_UINT32 (*this, EXIF_TAG_RELATED_IMAGE_WIDTH,       &exif->fRelatedImageWidth);
  SET_DNG_TAG_UINT32 (*this, EXIF_TAG_RELATED_IMAGE_LENGTH,      &exif->fRelatedImageLength);

  // Assign Raw image data.
  negative->SetStage1Image(image);

  // Compute linearized and range mapped image
  negative->BuildStage2Image(host);

  // Compute demosaiced image (used by preview and thumbnail)
  negative->BuildStage3Image(host);

  negative->SynchronizeMetadata();
  negative->RebuildIPTC(true, false);

  // DNGWriter: DNG thumbnail creation

  dng_preview_list previewList;

  dng_image_preview thumbnail;
  dng_render thumbnail_render(host, *negative);
  thumbnail_render.SetFinalSpace(dng_space_sRGB::Get());
  thumbnail_render.SetFinalPixelType(ttByte);
  thumbnail_render.SetMaximumSize(256);
  thumbnail.fImage.Reset(thumbnail_render.Render());

  // Creating DNG file

  dng_image_writer writer;
  dng_file_stream filestream(dngFilename.c_str(), true);

  writer.WriteDNG(host, filestream, *negative.Get(), thumbnail, ccUncompressed, &previewList);

}

