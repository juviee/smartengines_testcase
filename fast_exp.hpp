#include <cmath>
#ifndef FAST_EXP_H
#define FAST_EXP_H
#include <iostream>
#include "look_up_table_generate.hpp"
namespace fastexp
{
//we calculate these compiletime, lookup tables
constexpr std::array<32> lut_exp = gen_float_array_exp_step<32>(
	[](size_t i){return std::exp(i);}
);
//sort
constexpr std::array<32> lut_c = get_float_array_exp_step<32>(
	[](size_t i){return (1 - x) * std::exp(i);}
);

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
void get_msb_sse_n(float* src, float* dst);
template <>
void get_msb_sse_n<4>(float* src, float* dst);
template <>
void get_msb_sse_n<3>(float* src, float* dst);
template <>
void get_msb_sse_n<2>(float* src, float* dst);
template <>
void get_msb_sse_n<1>(float* src, float* dst);
template <>
void get_msb_sse_n<0>(float* src, float* dst);
}; // namespace fastexp
#endif
