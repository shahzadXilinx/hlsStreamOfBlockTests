#include "sw_reorder.hpp"
void reOrder16sw(int inArr[16], int outArr[16])
{
    for (int i = 0; i < 16; ++i)
    {
        outArr[i] = inArr[16-i-1];
    }
}
void reOrder8sw(int inArr[16], int outArr[16])
{
    int count=0;
    for (int k = 0; k < 16/8; ++k)
    {

        for (int i = 0; i < 8; ++i)
        {
            outArr[i+8*k] = inArr[(k+1)*8-i-1];

        }
    }
}
void reOrder4sw(int inArr[16], int outArr[16])
{
    for (int k = 0; k < 16/4; ++k)
    {

        for (int i = 0; i < 4; ++i)
        {
            outArr[i+4*k] = inArr[(k+1)*4-i-1];
        }
    }
}
void top_sw(int inArr[16], int outArr[16])
{
    int buff16_a[16];
    int buff16_b[16];
    reOrder16sw(inArr,buff16_a);
    reOrder8sw(buff16_a,buff16_b);
    reOrder4sw(buff16_b,outArr);
}

