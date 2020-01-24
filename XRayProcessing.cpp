// XRayProcessing_Phase2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdio>
#include <ctime>
#include "XRayProcessing.hpp"
#include "libtiff/tiffio.h"

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "Not enough arguments.\n";
		return 0;
	}
	const char* inpath = argv[1];
	const char* outpath = argv[2];

	struct Errors errors = { 0, "none" };
	struct TiffMetadata meta = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uint16_t** pixeldata;
	double** calcdata;

	clock_t start, finish;
	start = clock();

	pixeldata = readTiff(inpath, &meta, &errors);
	if (errors.iserror == 1)
	{
		std::cout << errors.error;
		freePixeldata(pixeldata, 1);
		return 0;
	}

	calcdata = calculateMurhot(pixeldata, &meta, &errors);
	if (errors.iserror == 1)
	{
		std::cout << errors.error;
		freeCalcdata(calcdata, 1);
		freePixeldata(pixeldata, meta.nrows);
		return 0;
	}

	/////////////////////////////////////////////////////////////////
	// This is where the magic needs to happen!
	/////////////////////////////////////////////////////////////////

	//correct_for_thickness(calcdata, &meta, &geometry, &errors);
	//if (errors.iserror == 1)
	//{
	//	printf(errors.error);
	//	free_calcdata(calcdata, meta.nrows);
	//	free_pixeldata(pixeldata, meta.nrows);
	//	return 0;
	//}

	createContrastOutput(calcdata, pixeldata, &meta, &errors);
	if (errors.iserror == 1)
	{
		std::cout << errors.error;
		freeCalcdata(calcdata, meta.nrows);
		freePixeldata(pixeldata, meta.nrows);
		return 0;
	}

	writeTiff(pixeldata, outpath, &meta, &errors);

	freeCalcdata(calcdata, meta.nrows);
	freePixeldata(pixeldata, meta.nrows);
	if (errors.iserror == 1)
	{
		std::cout << errors.error;
		return 0;
	}

	finish = clock();
    std::cout << "Elapsed time: " << (finish - start) << " ms\n";
	return 1;
}
