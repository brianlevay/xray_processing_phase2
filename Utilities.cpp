#include <cstdlib>
#include <cstdint>
#include "XRayProcessing.hpp"
#include "libtiff/tiffio.h"

void freePixeldata(uint16_t** pixeldata, uint32_t nrows)
{
	uint32_t i;
	for (i = 0; i < nrows; i++)
	{
		free(pixeldata[i]);
	}
	free(pixeldata);
	return;
}

void freeCalcdata(double** calcdata, uint32_t nrows)
{
	uint32_t i;
	for (i = 0; i < nrows; i++)
	{
		free(calcdata[i]);
	}
	free(calcdata);
	return;
}

void recordError(struct Errors* errors, const char* message)
{
	(*errors).iserror = 1;
	(*errors).error = message;
	return;
}