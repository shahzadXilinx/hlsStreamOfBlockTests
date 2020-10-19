#include "dut_pipo.hpp"
void reOrder16(int inArr[16], int outArr[16])
{
    for (int i = 0; i < 16; ++i)
    {
#pragma HLS pipeline II=1
        outArr[i] = inArr[16-i-1];
    }
}
void reOrder8(int inArr[16], int outArr[16])
{
    int count=0;
    for (int k = 0; k < 16/8; ++k)
    {

        for (int i = 0; i < 8; ++i)
        {
    #pragma HLS pipeline II=1
            outArr[i+8*k] = inArr[(k+1)*8-i-1];

        }
    }
}
void reOrder4(int inArr[16], int outArr[16])
{
    for (int k = 0; k < 16/4; ++k)
    {

        for (int i = 0; i < 4; ++i)
        {
    #pragma HLS pipeline II=1
            outArr[i+4*k] = inArr[(k+1)*4-i-1];
        }
    }
}
void top_pipo(int inArr[16], int outArr[16])
{
    int buff16_a[16];
    int buff16_b[16];
#pragma HLS dataflow
    reOrder16(inArr,buff16_a);
    reOrder8(buff16_a,buff16_b);
    reOrder4(buff16_b,outArr);
}
