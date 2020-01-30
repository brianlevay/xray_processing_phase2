#include <vector>
#include <cstdlib>
#include <cstdint>
#include <math.h>
#include "XRayProcessing.hpp"
#include "libtiff/tiffio.h"

std::vector<double> calculateMurhot(std::vector<uint16_t> &pixeldata, struct TiffMetadata* meta, struct Errors* errors)
{
	uint16_t pixelmax;
	uint32_t i, j, k, Itable, nrows, ncols;
	std::vector<double> calcdata;
	std::vector<double> murhot_table;
	
	nrows = (*meta).nrows;
	ncols = (*meta).ncols;
	pixelmax = (*meta).pixelmax;
	
	try
	{
		calcdata.resize(nrows * ncols);
	}
	catch (int e)
	{
		recordError(errors, "Unable to allocate enough memory for the image array\n");
		return calcdata;
	}
	try
	{
		murhot_table.resize(pixelmax + 1);
	}
	catch (int e)
	{
		recordError(errors, "Unable to allocate enough memory for the lookup table\n");
		return calcdata;
	}
	
	murhot_table[0] = -log((double)1 / (double)pixelmax);
	for (Itable = 1; Itable <= pixelmax; Itable++)
	{
		murhot_table[Itable] = -log((double)Itable / (double)pixelmax);
	}
	for (i = 0; i < nrows; i++)
	{
		for (j = 0; j < ncols; j++)
		{
			k = i * ncols + j;
			calcdata[k] = murhot_table[pixeldata[k]];
		}
	}
	return calcdata;
}