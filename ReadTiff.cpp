#include <cstdlib>
#include <cstdint>
#include "XRayProcessing.hpp"
#include "libtiff/tiffio.h"

uint16_t** readTiff(const char* inpath, struct TiffMetadata* meta, struct Errors* errors)
{
	uint16_t pixelmax;
	uint32_t i, j, nrows, ncols;
	int status;
	uint16_t** pixeldata_empty;
	uint16_t** pixeldata;
	
	pixeldata_empty = new uint16_t*[1];
	pixeldata_empty[0] = new uint16_t[1];
	
	TIFF* tif_in = TIFFOpen(inpath,"r");
	if (!tif_in) 
	{
		recordError(errors, "Unable to open tif file.\n");
		return pixeldata_empty;
	}
	
	TIFFGetFieldDefaulted(tif_in, TIFFTAG_IMAGEWIDTH, &(*meta).ncols);
	TIFFGetFieldDefaulted(tif_in, TIFFTAG_IMAGELENGTH, &(*meta).nrows);
	TIFFGetFieldDefaulted(tif_in, TIFFTAG_SAMPLESPERPIXEL, &(*meta).samplesperpixel);
	TIFFGetFieldDefaulted(tif_in, TIFFTAG_BITSPERSAMPLE, &(*meta).bitspersample);
	TIFFGetFieldDefaulted(tif_in, TIFFTAG_ORIENTATION, &(*meta).orientation);
	TIFFGetFieldDefaulted(tif_in, TIFFTAG_PLANARCONFIG, &(*meta).planarconfig);
	TIFFGetFieldDefaulted(tif_in, TIFFTAG_PHOTOMETRIC, &(*meta).photometric);
	TIFFGetFieldDefaulted(tif_in, TIFFTAG_COMPRESSION, &(*meta).compression);
	(*meta).scanlinesize = TIFFScanlineSize(tif_in);
	
	if (((*meta).samplesperpixel != 1) || ((*meta).bitspersample != 16) || ((*meta).planarconfig != PLANARCONFIG_CONTIG)) 
	{
		recordError(errors, "Image does not meet expectations for this code. Images must be contiguous 16-bit greyscale.\n");
		TIFFClose(tif_in);
		return pixeldata_empty;
	}
	
	nrows = (*meta).nrows;
	ncols = (*meta).ncols;
	pixeldata = new uint16_t*[nrows];
	if (!pixeldata)
	{
		recordError(errors, "Unable to allocate enough memory for the image array\n");
		TIFFClose(tif_in);
		return pixeldata_empty;
	}
	
	pixelmax = 0;
	for (i = 0; i < nrows; i++) 
	{
		pixeldata[i] = new uint16_t[ncols];
		if (!pixeldata[i])
		{
			recordError(errors, "Unable to allocate enough memory for the image array\n");
			TIFFClose(tif_in);
			freeJaggedArray(pixeldata, i);
			return pixeldata_empty;
		}
		status = TIFFReadScanline(tif_in, pixeldata[i], i, 0);
		if (status == -1)
		{
			recordError(errors, "Unable to read data from image array\n");
			TIFFClose(tif_in);
			freeJaggedArray(pixeldata, i);
			return pixeldata_empty;
		}
		for (j = 0; j < ncols; j++)
		{
			if (pixeldata[i][j] > pixelmax)
			{
				pixelmax = pixeldata[i][j];
			}
		}
	}
	(*meta).pixelmax = pixelmax;
	TIFFClose(tif_in);
	freeJaggedArray(pixeldata_empty, 1);
	return pixeldata;
}