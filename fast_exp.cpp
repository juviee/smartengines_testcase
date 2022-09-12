#include "fast_exp.hpp"
#include "look_up_table_generate.hpp"
#include <iostream>
#include <cmath>
#include <array>

int main()
{
    return 0;
}

void fast_exp_sse(float* in, float* out, size_t num)
{
    std::array lut_exp = gen_float_array_exp_step<32>(
        [](size_t i){return std::exp(i);}
        );
//sort
    std::array lut_c = get_float_array_exp_step<32>(
        [](size_t i){return (1 - x) * std::exp(i);}
        );
    // (e ^ x) = lut_c[~x] + lut_exp[~x] * x
    // where lut_c[] and lut_exp[] are compile_time calculated
    // ~x -- highest byte in x(at int form, MSB)
}

template<>
void fast_exp_sse_n<4>(float* in, float* out)
{

}

void get_msb_sse_n<4>(float* in, float* out)
{
    
}