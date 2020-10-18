#include <math.h>
#include <stdio.h>
#include <cstring>
#include <vector>

#include "dct.h"

namespace ref
{
	void DCT(float &d0, float &d1, float &d2, float &d3, float &d4, float &d5, float &d6, float &d7) 
	{
		float tmp0 = d0 + d7;
		float tmp7 = d0 - d7;
		float tmp1 = d1 + d6;
		float tmp6 = d1 - d6;
		float tmp2 = d2 + d5;
		float tmp5 = d2 - d5;
		float tmp3 = d3 + d4;
		float tmp4 = d3 - d4;

		// Even part
		float tmp10 = tmp0 + tmp3;	// phase 2
		float tmp13 = tmp0 - tmp3;
		float tmp11 = tmp1 + tmp2;
		float tmp12 = tmp1 - tmp2;

		d0 = tmp10 + tmp11; 		// phase 3
		d4 = tmp10 - tmp11;

		float z1 = (tmp12 + tmp13) * 0.707106781f; // c4
		d2 = tmp13 + z1; 		// phase 5
		d6 = tmp13 - z1;

		// Odd part
		tmp10 = tmp4 + tmp5; 		// phase 2
		tmp11 = tmp5 + tmp6;
		tmp12 = tmp6 + tmp7;

		// The rotator is modified from fig 4-8 to avoid extra negations.
		float z5 = (tmp10 - tmp12) * 0.382683433f; // c6
		float z2 = tmp10 * 0.541196100f + z5; // c2-c6
		float z4 = tmp12 * 1.306562965f + z5; // c2+c6
		float z3 = tmp11 * 0.707106781f; // c4

		float z11 = tmp7 + z3;		// phase 5
		float z13 = tmp7 - z3;

		d5 = z13 + z2;			// phase 6
		d3 = z13 - z2;
		d1 = z11 + z4;
		d7 = z11 - z4;
	} 

	void dct(float data[64]) 
	{
		// DCT rows
		for(int i=0; i<64; i+=8) {
			DCT(data[i], data[i+1], data[i+2], data[i+3], data[i+4], data[i+5], data[i+6], data[i+7]);
		}
		// DCT columns
		for(int i=0; i<8; ++i) {
			DCT(data[i], data[i+8], data[i+16], data[i+24], data[i+32], data[i+40], data[i+48], data[i+56]);
		}
	}

	void dct(unsigned int nblocks, float *data)
	{
		for (int i=0; i<nblocks; i++) {
			dct( &data[64*i] );
		}
	}

};

int main(int argc, char *argv[])
{
	unsigned int nvalues;
	unsigned int nblocks;

	if (argc>1)
		nblocks = atoi(argv[1]);
	else
		nblocks = 512;

	nvalues = nblocks*64;

	printf("Running test %d blocks (%d values)\n", nblocks, nvalues);

	std::vector<float>  ref_v(nvalues);
	hls::stream<float>  dut_i;
	hls::stream<float>  dut_o;

	float dat;
	for (int i=0; i<nvalues; i++) {
		dat = rand()%1024;
		dut_i.write(dat);
		ref_v[i] = dat;
	}

	ref::dct(nblocks, ref_v.data() );

	krnl_dct(nblocks, dut_i, dut_o );


	int err = 0;
	for (int i=0; i<nvalues; i++) {
		if (dut_o.read() != ref_v[i]) err++;
	}

	if (err)
		printf("Test finished with %d error(s)\n", err);
	else 
		printf("Test PASSED\n");

	return err;

}