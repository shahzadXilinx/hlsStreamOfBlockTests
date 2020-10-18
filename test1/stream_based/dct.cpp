#include "dct.h"
#include "hls_streamofblocks.h"

static void DCT(float &d0, float &d1, float &d2, float &d3, float &d4, float &d5, float &d6, float &d7,
	            float &t0, float &t1, float &t2, float &t3, float &t4, float &t5, float &t6, float &t7) 
{
#pragma HLS INLINE
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

	t0 = tmp10 + tmp11; 		// phase 3
	t4 = tmp10 - tmp11;

	float z1 = (tmp12 + tmp13) * 0.707106781f; // c4
	t2 = tmp13 + z1; 		// phase 5
	t6 = tmp13 - z1;

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

	t5 = z13 + z2;			// phase 6
	t3 = z13 - z2;
	t1 = z11 + z4;
	t7 = z11 - z4;
} 


typedef float block8x8[64]; 


void dct_1d_rows(unsigned int nblocks, hls::stream_of_blocks<block8x8> &i_buff, hls::stream_of_blocks<block8x8> &o_buff)
{
	for(int n=0; n<nblocks; n++) {
#pragma HLS loop_tripcount min=512 max=32400
#pragma HLS PIPELINE II=64

	    hls::read_lock<block8x8>  in(i_buff); 
	    hls::write_lock<block8x8> out(o_buff);

		dct_rows: for(int i=0; i<64; i+=8) {
			DCT(in[i], in[i+1], in[i+2], in[i+3], in[i+4], in[i+5], in[i+6], in[i+7],
				out[i], out[i+1], out[i+2], out[i+3], out[i+4], out[i+5], out[i+6], out[i+7]);
		}
	}
}

void dct_1d_cols(unsigned int nblocks, hls::stream_of_blocks<block8x8> &i_buff, hls::stream_of_blocks<block8x8> &o_buff)
{
	for(int n=0; n<nblocks; n++) {
#pragma HLS loop_tripcount min=512 max=32400
#pragma HLS PIPELINE II=64

	    hls::read_lock<block8x8>  in(i_buff); 
	    hls::write_lock<block8x8> out(o_buff);

		dct_cols: for(int i=0; i<8; ++i) {
			DCT(in[i], in[i+8], in[i+16], in[i+24], in[i+32], in[i+40], in[i+48], in[i+56],
				out[i], out[i+8], out[i+16], out[i+24], out[i+32], out[i+40], out[i+48], out[i+56]);
		}
	}
}

void stream2buff(unsigned int nblocks, hls::stream<float> &stream, hls::stream_of_blocks<block8x8> &o_buff)
{
	for(int n=0; n<nblocks; n++) {
#pragma HLS loop_tripcount min=512 max=32400
#pragma HLS PIPELINE II=64

	    hls::write_lock<block8x8> buff(o_buff);

		stream2buff_l: for(int i=0; i<64; i++) {
			buff[i] = stream.read();
		}	
	}
}

void buff2stream(unsigned int nblocks, hls::stream_of_blocks<block8x8> &i_buff, hls::stream<float> &stream)
{
	for(int n=0; n<nblocks; n++) {
#pragma HLS loop_tripcount min=512 max=32400
#pragma HLS PIPELINE II=64

	    hls::read_lock<block8x8>  buff(i_buff); 

		buff2stream_l: for(int i=0; i<64; i++) {
			stream.write(buff[i]);
		}	
	}
}

void krnl_dct(
	unsigned int           nblocks,
	hls::stream<float>    &data_i,
	hls::stream<float>    &data_o )
{

#pragma HLS DATAFLOW

	hls::stream_of_blocks<block8x8> dct_in;
	hls::stream_of_blocks<block8x8> dct_buf;
	hls::stream_of_blocks<block8x8> dct_out;

	stream2buff(nblocks, data_i,  dct_in);
	dct_1d_rows(nblocks, dct_in , dct_buf);
	dct_1d_cols(nblocks, dct_buf, dct_out);
	buff2stream(nblocks, dct_out, data_o);
}
