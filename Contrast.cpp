#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <math.h>
#include "XRayProcessing.hpp"
#include "libtiff/tiffio.h"

void createContrastOutput(std::vector<double> &calcdata, std::vector<uint16_t> &pixeldata, struct TiffMetadata* meta, struct Errors* errors)
{
	uint16_t pixelmax, maxuint16;
	uint32_t i, j, k, k_tbl, nrows, ncols;
	double stretch_p, maxuint16_dbl;
	double calc_lower, calc_upper;
	double Lstep, Ltable, stretch_table_m;
	double table_index_m, k_tbl_dbl;
	std::vector<double> data_limits;
	std::vector<uint16_t> stretch_table;
	
	nrows = (*meta).nrows;
	ncols = (*meta).ncols;
	pixelmax = (*meta).pixelmax;
	stretch_p = STRETCH_P;
	maxuint16 = MAX_UINT16;
	maxuint16_dbl = (double) maxuint16;
	
	data_limits = calculatePixelLimits(pixeldata, meta, errors);
	if ((*errors).iserror == 1)
	{
		return;
	}
	try
	{
		stretch_table.resize(maxuint16 + 1);
	}
	catch (int e)
	{
		recordError(errors, "Unable to allocate memory for stretch_table.\n");
		return;
	}
	
	calc_lower = -log(data_limits[1] / (double)pixelmax);
	calc_upper = -log(data_limits[0] / (double)pixelmax);
	
	Lstep = (1.0 - 0.0) / (maxuint16_dbl - 0.0);
	stretch_table_m = (0.0 - maxuint16_dbl)/(1.0 - 0.0);
	for (k = 0; k <= maxuint16; k++)
	{
		Ltable = (double)k * Lstep;
		stretch_table[k] = (uint16_t) (stretch_table_m * pow(Ltable, stretch_p) + maxuint16_dbl);
	}
	
	table_index_m = ((1.0 - 0.0) / (calc_upper - calc_lower)) * (1.0 / Lstep);
	for (i = 0; i < nrows; i++)
	{
		for (j = 0; j < ncols; j++)
		{
			k = i * ncols + j;
			k_tbl_dbl = table_index_m * (calcdata[k] - calc_lower);
			if (k_tbl_dbl < 0.0)
			{
				k_tbl_dbl = 0;
			}
			if (k_tbl_dbl > maxuint16_dbl)
			{
				k_tbl_dbl = maxuint16_dbl;
			}
			k_tbl = (uint16_t)k_tbl_dbl;
			pixeldata[k] = stretch_table[k_tbl];
		}
	}
	
	return;
}