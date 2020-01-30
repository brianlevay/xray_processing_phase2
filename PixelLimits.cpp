#include <vector>
#include <cstdlib>
#include <cstdint>
#include <math.h>
#include "XRayProcessing.hpp"
#include "libtiff/tiffio.h"

std::vector<double> calculatePixelLimits(std::vector<uint16_t> &pixeldata, struct TiffMetadata* meta, struct Errors* errors)
{
	uint16_t lower_offset_I, nshift;
	uint32_t i, j, k, n, nrows, ncols, nbins, bin_step;
	double cumulative, cumulative_min, freq_min;
	std::vector<double> data_limits;
	std::vector<double> hist_freq;
	
	nrows = (*meta).nrows;
	ncols = (*meta).ncols;
	nbins = NBINS;
	bin_step = BIN_STEP;
	cumulative_min = CUMULATIVE_MIN;
	lower_offset_I = LOWER_OFFSET_I;
	freq_min = FREQ_MIN;
	
	data_limits.resize(2);
	try
	{
		hist_freq.resize(nbins);
	}
	catch (int e)
	{
		recordError(errors, "Unable to allocate memory for hist_freq.\n");
		return data_limits;
	}
	
	for (i = 0; i < nrows; i++)
	{
		for (j = 0; j < ncols; j++)
		{
			k = i * ncols + j;
			nshift = pixeldata[k] >> 8;
			hist_freq[nshift] = hist_freq[nshift] + 1.0;
		}
	}
	cumulative = 0.0;
	for (n = 0; n < nbins; n++)
	{
		cumulative = cumulative + hist_freq[n];
	}
	for (n = 0; n < nbins; n++)
	{
		hist_freq[n] = hist_freq[n] / cumulative;
	}
	
	cumulative = 0.0;
	data_limits[0] = 0.0;
	data_limits[1] = (double)nbins * (double)bin_step - 1.0;
	for (n = 0; n < nbins; n++)
	{
		cumulative = cumulative + hist_freq[n];
		if ((cumulative <= cumulative_min) && (hist_freq[n] <= freq_min))
		{
			data_limits[0] = (double)n * (double)bin_step;
		}
		if ((cumulative >= (1.0 - cumulative_min)) && (hist_freq[n] <= freq_min))
		{
			data_limits[1] = (double)n * (double)bin_step - 1.0;
			break;
		}
	}
	data_limits[0] = data_limits[0] - (double)lower_offset_I;
	if (data_limits[0] < 0.0)
	{
		data_limits[0] = 0.0;
	}
	
	return data_limits;
}