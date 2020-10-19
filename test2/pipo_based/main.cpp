#include <iostream>
#include "dut_pipo.hpp"
#include "sw_reorder.hpp"

int main(int argc, char **argv)
{
    int in[16];
    int out[16];
    int out_sw[16];
    int out_pipo[16];

    for (int i = 0; i < 16; ++i)
    {
        in[i] = i;
    }
    top_sw(in,out_sw);
    top_pipo(in,out);
    for (int i = 0; i < 16; ++i)
    {
        if(out[i] != out_sw[i])
        {
            std::cout<<"Output Mismatch at index : "<<i<<std::endl;
            std::cout<<"Expected Output was : " <<out_sw[i]<<std::endl;
            std::cout<<"Produced Output is  : " <<out[i]<<std::endl;
            return -1;
        }
    }
    std::cout<<"Ouput matches the Expected output, simulation completed Succesfully ..."<<std::endl;

    return 0;
}
