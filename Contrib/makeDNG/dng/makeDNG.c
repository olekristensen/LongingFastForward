
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include <tiffio.h>

#define ROT(x) (x)

#define TIFFTAG_AnalogBalance 50727

int main (int argc, char **argv)
{
	static const short CFARepeatPatternDim[] = { 2,2 };
	static const float cam_xyz[] =
   // { 2.005,-0.771,-0.269, -0.752,1.688,0.064, -0.149,0.283,0.745 }; // default
    // {3.2404542, -1.5371385, -0.4985314, -0.9692660,  1.8760108,  0.0415560,0.0556434, -0.2040259,  1.0572252 }; // sRGB
    { 2.0413690, -0.5649464, -0.3446944, -0.9692660,  1.8760108,  0.0415560,0.0134474, -0.1183897,  1.0154096 }; // Adobe RGB
	static const float neutral[] = { 1.0, 1.0, 1.0};
    static const float balance[] = { 1.5, 1.0, 1.5};
    long sub_offset=0, white=0xfff, black=0;
	float gam;
	int status=1, i, r, c, row, col;
	unsigned short curve[256];
	unsigned char *out;
	struct stat st;
	struct tm tm;
	char datetime[64];
	FILE *ifp;
	TIFF *tif;
	
	for (i=0; i < 256; i++)
		curve[i] = 0xfff * pow (i/255.0,1) + 0.5;
	
	if (!(ifp = fopen (argv[1], "rb"))) {
		perror (argv[1]);
		return 1;
	}
	stat (argv[1], &st);
	gmtime_r (&st.st_mtime, &tm);
	sprintf (datetime, "%04d:%02d:%02d %02d:%02d:%02d",
			 tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
	
	int width=atof(argv[2]);
	int height=atof(argv[3]);
	if (!(tif = TIFFOpen (argv[4], "w"))) goto fail;
	
	TIFFSetField (tif, TIFFTAG_SUBFILETYPE, 1);
	TIFFSetField (tif, TIFFTAG_IMAGEWIDTH, width>>4);
	TIFFSetField (tif, TIFFTAG_IMAGELENGTH, height>>4);
	TIFFSetField (tif, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField (tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
	TIFFSetField (tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	TIFFSetField (tif, TIFFTAG_MAKE, "Prosilica/AVT");
	TIFFSetField (tif, TIFFTAG_MODEL, "GX1910C");
	TIFFSetField (tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField (tif, TIFFTAG_SAMPLESPERPIXEL, 3);
	TIFFSetField (tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField (tif, TIFFTAG_SOFTWARE, "field_dng");
	TIFFSetField (tif, TIFFTAG_DATETIME, datetime);
	TIFFSetField (tif, TIFFTAG_SUBIFD, 1, &sub_offset);
	TIFFSetField (tif, TIFFTAG_DNGVERSION, "\001\001\0\0");
	TIFFSetField (tif, TIFFTAG_DNGBACKWARDVERSION, "\001\0\0\0");
	TIFFSetField (tif, TIFFTAG_UNIQUECAMERAMODEL, "GX1910C");
	TIFFSetField (tif, TIFFTAG_COLORMATRIX1, 9, cam_xyz);
	TIFFSetField (tif, TIFFTAG_ASSHOTNEUTRAL, 3, neutral);
//  TIFFSetField (tif, TIFFTAG_ASSHOTWHITEXY, 2, whitebal);
    TIFFSetField (tif, TIFFTAG_CALIBRATIONILLUMINANT1, 21); //D65
    TIFFSetField (tif, TIFFTAG_AnalogBalance, 3,balance);
//  TIFFSetField (tif, TIFFTAG_ORIGINALRAWFILENAME, argv[2]);
	
	char *blackBuf = (char *)malloc(width>>4);
	
	
	memset (blackBuf, 0, width>>4);// all-black thumbnail
	for (row=0; row < height>> 4; row++)
//		TIFFWriteScanline (tif, blackBuf, row, 0);
//	TIFFWriteDirectory (tif);
	
	TIFFSetField (tif, TIFFTAG_SUBFILETYPE, 0);
	TIFFSetField (tif, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField (tif, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField (tif, TIFFTAG_BITSPERSAMPLE, 16);
	TIFFSetField (tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_CFA);
	TIFFSetField (tif, TIFFTAG_SAMPLESPERPIXEL, 1);
	TIFFSetField (tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField (tif, TIFFTAG_CFAREPEATPATTERNDIM, CFARepeatPatternDim);
	TIFFSetField (tif, TIFFTAG_CFAPATTERN, 4, "\001\0\02\001");
 //   TIFFSetField (tif, 50738, 0.0); // AntiAliasStrength
	TIFFSetField (tif, TIFFTAG_WHITELEVEL, 1, &white);
	TIFFSetField (tif, TIFFTAG_BLACKLEVEL, 1, &black);
	
	out = calloc (width*2, 1);
	
	for (row=0; row < height; row ++) {
		for(col=0;col<width*2;col++)
		{
			int m = 0;
			out[col+m] = fgetc(ifp);
		}
		
		TIFFWriteScanline (tif, out, row, 0);
	}
	
	free (out);
	TIFFClose (tif);
	status = 0;
fail:
	fclose (ifp);
	return status;
}
