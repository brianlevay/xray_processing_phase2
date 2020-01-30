#include <cstdlib>
#include <cstdint>
#include "XRayProcessing.hpp"
#include "libtiff/tiffio.h"

void recordError(struct Errors* errors, const char* message)
{
	(*errors).iserror = 1;
	(*errors).error = message;
	return;
}