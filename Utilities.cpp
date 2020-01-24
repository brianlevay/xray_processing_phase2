#include <cstdlib>
#include <cstdint>
#include "XRayProcessing.hpp"
#include "libtiff/tiffio.h"

void freeJaggedArray(uint16_t** data, uint32_t nrows)
{
	uint32_t i;
	for (i = 0; i < nrows; i++)
	{
		delete[] data[i];
	}
	delete[] data;
	return;
}

void freeJaggedArray(double** data, uint32_t nrows)
{
	uint32_t i;
	for (i = 0; i < nrows; i++)
	{
		delete[] data[i];
	}
	delete[] data;
	return;
}

void recordError(struct Errors* errors, const char* message)
{
	(*errors).iserror = 1;
	(*errors).error = message;
	return;
}