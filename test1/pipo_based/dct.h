#pragma once

#include "hls_stream.h"

void krnl_dct(
	unsigned int           nblocks,
	hls::stream<float>    &data_i,
	hls::stream<float>    &data_o );