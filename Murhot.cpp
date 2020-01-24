#include <cstdlib>
#include <cstdint>
#include <math.h>
#include "XRayProcessing.hpp"
#include "libtiff/tiffio.h"

double** calculateMurhot(uint16_t** pixeldata, struct TiffMetadata* meta, struct Errors* errors)
{
	uint16_t pixelmax;
	uint32_t i, j, Itable, nrows, ncols;
	double* murhot_table;
	double** calcdata_empty;
	double** calcdata;
	
	nrows = (*meta).nrows;
	ncols = (*meta).ncols;
	pixelmax = (*meta).pixelmax;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Memory allocation and error handling 
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	calcdata_empty = (double**) malloc(1 * sizeof(double*));
	calcdata_empty[0] = (double*) malloc(1 * sizeof(double));
	
	murhot_table = (double*) malloc((pixelmax + 1) * sizeof(double));
	if (!murhot_table)
	{
		recordError(errors, "Unable to allocate memory for murhot_table.\n");
		return calcdata_empty;
	}
	calcdata = (double**) malloc(nrows * sizeof(double*));
	if (!calcdata)
	{
		recordError(errors, "Unable to allocate memory for calcdata.\n");
		free(murhot_table);
		return calcdata_empty;
	}
	for (i = 0; i < nrows; i++) 
	{
		calcdata[i] = (double*) malloc(ncols * sizeof(double));
		if (!calcdata[i])
		{
			recordError(errors, "Unable to allocate enough memory for calcdata\n");
			freeCalcdata(calcdata, i);
			free(murhot_table);
			return calcdata_empty;
		}
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Calculations
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	murhot_table[0] = -log((double)1 / (double)pixelmax);
	for (Itable = 1; Itable <= pixelmax; Itable++)
	{
		murhot_table[Itable] = -log((double)Itable / (double)pixelmax);
	}
	
	for (i = 0; i < nrows; i++)
	{
		for (j = 0; j < ncols; j++)
		{
			calcdata[i][j] = murhot_table[pixeldata[i][j]];
		}
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Memory release
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	free(murhot_table);
	freeCalcdata(calcdata_empty, 1);
	return calcdata;
}