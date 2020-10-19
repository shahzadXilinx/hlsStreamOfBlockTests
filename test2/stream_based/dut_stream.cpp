#include "dut_stream.hpp"

template < int N>
void arr2Stream(int in[N],hls::stream<int> &outStream)
{
    for (int i = 0; i < N; ++i) {
        outStream.write(in[i]);
    }
}


template < int N>
void stream2Arr(hls::stream<int> &inStream,int out[N])
{
    for (int i = 0; i < N; ++i) {
        out[i] = inStream.read();
    }
}



template< int N>
void rev(int in[N], int out[N])
{
    for (int i = 0; i < N; ++i)
    {
        out[i] = in[N-1-i];
    }
}



void rev16(int in[16],hls::stream<int> &outStream)
{
    for (int i = 0; i < 16; ++i)
    {
        outStream.write(in[16-1-i]);
    }
}

void rev8(hls::stream<int> &inStream,hls::stream<int> &outStream)
{
    int buff[8];
    int buff2[8];
    for (int k = 0; k < 16/8; ++k)
    {

        #pragma HLS dataflow
            stream2Arr<8>(inStream,buff);
            rev<8>(buff,buff2);
            arr2Stream<8>(buff2,outStream);
    }
}

void rev4(hls::stream<int> &inStream,hls::stream<int> &outStream)
{
    int buff[4];
    int buff2[4];
    for (int k = 0; k < 16/4; ++k)
    {
        #pragma HLS dataflow
            stream2Arr<4>(inStream,buff);
            rev<4>(buff,buff2);
            arr2Stream<4>(buff2,outStream);
    }
}

void top_stream(int in[16], int out[16])
{
#pragma HLS dataflow
    hls::stream<int> rev16OutStream;
    hls::stream<int> rev8OutStream;
    hls::stream<int> rev4OutStream;
    rev16(in,rev16OutStream);
    rev8(rev16OutStream,rev8OutStream);
    rev4(rev8OutStream,rev4OutStream);
    stream2Arr<16>(rev4OutStream,out);
}
