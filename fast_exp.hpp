#include <cmath>
#ifndef FAST_EXP_H
#define FAST_EXP_H
#include <iostream>

void fast_exp_sse(float* src, float* dst, size_t num);
template<size_t n>
void fast_exp_sse_n(float* src, float* dst);
template<>
void fast_exp_sse_n<4>(float* src, float* dst);
#endif