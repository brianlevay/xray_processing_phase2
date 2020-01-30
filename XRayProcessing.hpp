#include <vector>
#include <cstdint>
#include "libtiff/tiffio.h"

#ifndef XRAY_MODEL_H
#define XRAY_MODEL_H

#define MAX_UINT16 65535
#define NBINS 256
#define BIN_STEP 256
#define FREQ_MIN 0.001
#define CUMULATIVE_MIN 0.02
#define LOWER_OFFSET_I 500
#define STRETCH_P 2.0

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Structs
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	
struct Errors
{
	int iserror;
	const char* error;
};

struct TiffMetadata
{
	uint32_t nrows;
	uint32_t ncols;
	uint16_t pixelmax;
	uint16_t samplesperpixel;
	uint16_t bitspersample;
	uint16_t orientation;
	uint16_t planarconfig;
	uint16_t photometric;
	uint16_t compression;
	tsize_t scanlinesize;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Function declarations
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void recordError(struct Errors* errors, const char* message);

void freeJaggedArray(uint16_t** data, uint32_t nrows);

void freeJaggedArray(double** data, uint32_t nrows);

std::vector<uint16_t> readTiff(const char* inpath, struct TiffMetadata* meta, struct Errors* errors);

void writeTiff(std::vector<uint16_t> pixeldata, const char* outpath, struct TiffMetadata* meta, struct Errors* errors);

double** calculateMurhot(uint16_t** pixeldata, struct TiffMetadata* meta, struct Errors* errors);

double* calculatePixelLimits(uint16_t** pixeldata, struct TiffMetadata* meta, struct Errors* errors);

void createContrastOutput(double** calcdata, uint16_t** pixeldata, struct TiffMetadata* meta, struct Errors* errors);

#endif