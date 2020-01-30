#include <vector>
#include <cstdlib>
#include <cstdint>
#include "XRayProcessing.hpp"
#include "libtiff/tiffio.h"

void writeTiff(std::vector<uint16_t> pixeldata, const char* outpath, struct TiffMetadata* meta, struct Errors* errors)
{
	int status, ret;
	uint32_t i, j, k, nrows, ncols;
	
	TIFF* tif_out = TIFFOpen(outpath, "w");
	if (!tif_out) 
	{
		recordError(errors, "Unable to create new tif file.\n");
		return;
	}
	
	TIFFSetField(tif_out, TIFFTAG_IMAGEWIDTH, (*meta).ncols);
	TIFFSetField(tif_out, TIFFTAG_IMAGELENGTH, (*meta).nrows);
	TIFFSetField(tif_out, TIFFTAG_SAMPLESPERPIXEL, (*meta).samplesperpixel);
	TIFFSetField(tif_out, TIFFTAG_BITSPERSAMPLE, (*meta).bitspersample);
	TIFFSetField(tif_out, TIFFTAG_ORIENTATION, (*meta).orientation);
	TIFFSetField(tif_out, TIFFTAG_PLANARCONFIG, (*meta).planarconfig);
	TIFFSetField(tif_out, TIFFTAG_PHOTOMETRIC, (*meta).photometric);
	TIFFSetField(tif_out, TIFFTAG_COMPRESSION, (*meta).compression);
	
	nrows = (*meta).nrows;
	ncols = (*meta).ncols;

	for (i = 0; i < nrows; i++) {
		k = i * ncols;
		status = TIFFWriteScanline(tif_out, &pixeldata[k], i, 0);
		if (status == -1)
		{
			recordError(errors, "Unable to write to tif file.\n");
			break;
		}
	}
	
	TIFFClose(tif_out);
	
	if ((*errors).iserror == 1)
	{
		ret = remove(outpath);
		if (ret == -1)
		{
			recordError(errors, "Unable to write to tif file, and unable to delete remnants.\n");
		}
	}
	return;
}
