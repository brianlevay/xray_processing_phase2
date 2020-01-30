#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstdint>
#include "XRayProcessing.hpp"
#include "libtiff/tiffio.h"

std::vector<uint16_t> readTiff(const char* inpath, struct TiffMetadata* meta, struct Errors* errors)
{
	int status;
	uint16_t pixelmax;
	uint32_t i, j, k, nrows, ncols;
	std::vector<uint16_t> pixeldata;
	
	TIFF* tif_in = TIFFOpen(inpath,"r");
	if (!tif_in) 
	{
		recordError(errors, "Unable to open tif file.\n");
		return pixeldata;
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
		return pixeldata;
	}
	
	nrows = (*meta).nrows;
	ncols = (*meta).ncols;
	try
	{
		pixeldata.resize(nrows * ncols);
	}
	catch (int e)
	{
		recordError(errors, "Unable to allocate enough memory for the image array\n");
		TIFFClose(tif_in);
		return pixeldata;
	}
	
	pixelmax = 0;
	for (i = 0; i < nrows; i++) 
	{
		k = i * ncols;
		status = TIFFReadScanline(tif_in, &pixeldata[k], i, 0);
		if (status == -1)
		{
			recordError(errors, "Unable to read data from image array\n");
			TIFFClose(tif_in);
			return pixeldata;
		}
		for (j = 0; j < ncols; j++)
		{
			if (pixeldata[k + j] > pixelmax)
			{
				pixelmax = pixeldata[k + j];
			}
		}
	}
	(*meta).pixelmax = pixelmax;
	TIFFClose(tif_in);
	return pixeldata;
}