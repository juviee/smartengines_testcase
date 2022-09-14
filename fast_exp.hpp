#ifndef FAST_EXP_H
#define FAST_EXP_H
#include "gcem.hpp"
#include <iostream>
#include "look_up_table_generate.hpp"

namespace fastexp
{
//we calculate these compiletime, lookup tables
constexpr std::array<float, 15> lut_exp = gen_float_array_exp_step<14>(
    [](int64_t i){return gcem::exp(i);} 
);
constexpr std::array<float, 9> lut_k = gen_float_arr_lookup_exp_step<8>(
    [](int64_t i, int64_t ii){
        return (gcem::exp(ii) - gcem::exp(i)) / (ii - i);
    });
// exp(1 << 15) is out of max(size_t) so stops being constexpr val
//sort
// constexpr std::array<float, 15> lut_c = gen_float_array_exp_step<14>(
// 	[](int64_t i){return (1 - i) * gcem::exp(i);}
// );

constexpr std::array<float, 9> lut_c = gen_float_arr_lookup_exp_step<8>(
    [](int64_t i, int64_t ii){
        return gcem::exp(i) - i * (gcem::exp(ii) - gcem::exp(i)) / (ii - i);
    });
inline float get_lut_exp_val( int32_t i )
{
    return (i > 14? 0 : lut_exp[i]);
}
inline float get_lut_k_val( int32_t i )
{
//we have three regions:
//x < 0 => fast_exp(x) == 1
//x >= 0 && x < 1 (no log2 for them) => fast_exp->(1 2.7)
//x >= 1 => fast_exp 
    if( i >= -1 && i < 8 )
        return lut_k[i+1];
    if(i >= 8)
        return lut_k[8];
    return 0;
}
inline float get_lut_c_val( int32_t i )
{
    if( i >= -1 && i < 8 )
        return lut_c[i+1];
    if(i >= 8)
        return lut_c[8];
    return 1;
}

void fast_exp_sse(float* src, float* dst, size_t num);
// fast_exp_sse_n

// loops over dst array calling <4>
// then calls appropriate for leftovers
template<size_t n>
void fast_exp_sse_n(float* src, float* dst);
template<>
void fast_exp_sse_n<4>(float* src, float* dst);
template<>
void fast_exp_sse_n<3>(float* src, float* dst);
template<>
void fast_exp_sse_n<2>(float* src, float* dst);
template<>
void fast_exp_sse_n<1>(float* src, float* dst);
template<>
void fast_exp_sse_n<0>(float* src, float* dst);

// getting msb needed for choosing appropriate
// lut_exp and lut_c
template <size_t n>
void get_msb_sse_n(float* src, int32_t* dst);
template <>
void get_msb_sse_n<4>(float* src, int32_t* dst);
template <>
void get_msb_sse_n<3>(float* src, int32_t* dst);
template <>
void get_msb_sse_n<2>(float* src, int32_t* dst);
template <>
void get_msb_sse_n<1>(float* src, int32_t* dst);
template <>
void get_msb_sse_n<0>(float* src, int32_t* dst);

inline int32_t fallback_mse( float x );
static const char LogTable256[256] = 
{
#define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
    -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
    LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
};

}; // namespace fastexp
#endif
