#include "fast_exp.hpp"
#include "immintrin.h"
#include <iostream>
#include <cmath>
#include <cstring>
#include <array>

namespace fastexp
{
// ***********
// fastexp sse
// ***********
void fast_exp_sse(float* in, float* out, size_t size)
{
    // (e ^ x) = lut_c[~x] + lut_exp[~x] * x
    // where lut_c[] and lut_exp[] are compile_time calculated
    // ~x -- highest byte in x(at int form, MSB)
    size_t pos = 0;
    for(; pos < size - 3; pos+=4)
    {
        fast_exp_sse_n<4>(in+pos, out+pos); // filling out w/ sse func 
    }
    size_t remaining = size -  pos;
    switch( remaining )
    // that will call one of fast_exp_sse_n<0..3>
    {
        case 0: 
        {
            break;
        }
        case 1: 
        {
            fast_exp_sse_n<1>(in+pos, out+pos);
            break;
        }
        case 2: 
        {
            fast_exp_sse_n<2>(in+pos, out+pos);
            break;
        }
        case 3: 
        {
            fast_exp_sse_n<3>(in+pos, out+pos);
            break;
        }
        default:
        {
            std::cerr << "remaining > 4!" << std::endl;
            break;
        }
    }
    return;
}

// **************************************
// template vectorized fast_exp using sse
// **************************************
template<size_t n>
void fast_exp_sse_n(float* in, float* out)
{
    // dummy for size_t n tmplt
    // use specifications!
    return;
}
template<>
void fast_exp_sse_n<4>(float* in, float* out)
{
    int32_t msb[4];
    get_msb_sse_n<4>(in, msb);
    // trim msb: if msb[i] > 14 => msb[i] = 0
    __m128 c4 = _mm_set_ps( get_lut_c_val(msb[3]), 
                            get_lut_c_val(msb[2]),
                            get_lut_c_val(msb[1]),
                            get_lut_c_val(msb[0])); // y offset at msb point

    __m128 e4 = _mm_set_ps( get_lut_k_val(msb[3]), 
                            get_lut_k_val(msb[2]),
                            get_lut_k_val(msb[1]),
                            get_lut_k_val(msb[0])); // exp at msb point

    __m128 x4 = _mm_load_ps( in ); // x_pos from (in)
                               // m8 be better using stream_load
    e4 = _mm_mul_ps( e4, x4 ); // lut_k(~x) * x (broadcasting, not dot)
    c4 = _mm_add_ps( e4, c4 ); // lut_c(~x) + lut_k(~x) * x
    _mm_store_ps( out, c4 );
}

template<>
void fast_exp_sse_n<3>(float* in, float* out)
{
    int32_t msb[4];
    get_msb_sse_n<3>(in, msb);
    __m128 c4 = _mm_set_ps( 0, get_lut_c_val(msb[2]), 
                            get_lut_c_val(msb[1]),
                            get_lut_c_val(msb[0])
                            ); // y offset at msb point

    __m128 e4 = _mm_set_ps( in[2], get_lut_k_val(msb[2]), 
                            get_lut_k_val(msb[1]),
                            get_lut_k_val(msb[0])
                            ); // exp at msb point

    __m128 x4 = _mm_castpd_ps(
                _mm_loadl_pd( _mm_castps_pd(e4), (double*) in )
                );
    x4 = _mm_castsi128_ps(
         _mm_shuffle_epi32(
         _mm_castps_si128(x4), 0b10110100
         ));

    //  x4 = [ in[0] : in[1] : e4[3] == in[2] : e4[2] == lut_exp[..2] ]
    // x4[3] is insignificant


    e4 = _mm_mul_ps( e4, x4 ); // lut_exp(~x) * x (broadcasting, not dot)
    c4 = _mm_add_ps( e4, c4 ); // lut_c(~x) + lut_exp(~x) * x
    float tmp_arr[4];
    _mm_store_ps( tmp_arr, c4 );
    for(size_t i = 0; i < 3; ++i)
    {
        out[i] = tmp_arr[i];
    }
}

template<>
void fast_exp_sse_n<2>(float* in, float* out)
{
    int32_t msb[4];
    get_msb_sse_n<2>(in, msb);
    __m128 c4 = _mm_set_ps(0, 0, get_lut_c_val(msb[1]), 
                            get_lut_c_val(msb[0])
                           ); // y offset at msb point

    __m128 e4 = _mm_set_ps( 0, 0, get_lut_k_val(msb[1]), 
                            get_lut_k_val(msb[0])
                            ); // exp at msb point

    __m128 x4 = _mm_castpd_ps(
            _mm_loadl_pd( _mm_castps_pd(e4), (double*) in )
            );

    //  x4 = [ in[0] : in[1] : e4[3] == 0 : e4[2] == 0 ]
    e4 = _mm_mul_ps( e4, x4 ); // lut_exp(~x) * x (broadcasting, not dot)
    c4 = _mm_add_ps( e4, c4 ); // lut_c(~x) + lut_exp(~x) * x
    float tmp_arr[4];
    _mm_store_ps( tmp_arr, c4 );
    for(size_t i = 0; i < 2; ++i)
    {
        out[i] = tmp_arr[i];
    }
    return;
}

template<>
void fast_exp_sse_n<1>(float* in, float* out)
{
    int32_t msb_val = 0;
    int32_t* msb = &msb_val;
    
    get_msb_sse_n<1>( in, msb );
    *out = get_lut_c_val(msb_val) + get_lut_k_val(msb_val) * (*in);
    return;
}

template<>
void fast_exp_sse_n<0>(float* in, float* out)
{
    return;
}

// *********************
// MSB template function 
// *********************
// additional_memory......
// ref: https://graphics.stanford.edu/~seander/bithacks.html
// By Sean Eron Anderson

template<size_t n>
void get_msb_sse_n(float* in, int32_t* out)
{
    // dummy for size_t n tmplt
    // use specifications!
    return;
}

template<>
void get_msb_sse_n<4>(float* in, int32_t* out)
{
    __m128 x4 = _mm_load_ps( in ); // x_pos from (in)
    __m128i zero_val_reg = _mm_setzero_si128(); // zero register
    __m128 cmp_mask = _mm_cmpgt_ps(x4, _mm_castsi128_ps(zero_val_reg));
    // mask (if x4[i] > 0)
    __m128i c4 = _mm_srli_epi32( _mm_castps_si128(x4), 23);

    __m128i delta_127 = _mm_set_epi32(127, 127, 127, 127);
    c4 = _mm_sub_epi32(c4, delta_127);

    c4 = _mm_and_si128( c4,
                        _mm_castps_si128(cmp_mask) );
    // zero if <0

    _mm_store_si128((__m128i*)out, c4);

    uint32_t flag_gt_zero[4]; // actualy gt 0
    _mm_store_ps((float*)flag_gt_zero, cmp_mask);
    for(size_t i = 0; i < 4; ++i)
    {
        if( !flag_gt_zero[i] )
        {
            if( in[i] < 0 )
                out[i] = -2;
            else
                out[i] = -1;
        }
        else if( (out[i] < 0) && (flag_gt_zero[i]) ) 
            // neq 0 -- mask value, but abnormal type
            out[i] = fallback_mse(in[i]);
    }
    return;
}

template<>
void get_msb_sse_n<3>(float* in, int32_t* out)
{
    for(size_t i = 0; i < 3; ++i)
        get_msb_sse_n<1>(in + i, out + i);
    return;
}

template<>
void get_msb_sse_n<2>(float* in, int32_t* out)
{
    for(size_t i = 0; i < 2; ++i)
        get_msb_sse_n<1>(in + i, out + i);
    return;
}

template<>
void get_msb_sse_n<1>(float* in, int32_t* out)
{
    const float v = *in;  // find int(log2(v)), where v > 0.0 && finite(v)
    if(v < 0) 
    {
        *out = 0;
        return;
    }

    uint32_t x; memcpy(&x, &v, sizeof x);
    uint32_t c = x >> 23;          
    if (c)
        c -= 127;
    else
    { 
        // subnormal, so recompute using mantissa: c = intlog2(x) - 149;
        c = fallback_mse(x);
    }
    *out = static_cast<int32_t>(c);
}

template<>
void get_msb_sse_n<0>(float* in, int32_t* out)
{
    return;
}

inline int32_t fallback_mse(float v_)
{
// we try to avoid that, although with subnormal vals we meet problems
    int32_t c_ = 0;
    int32_t x_ = floor(v_);
    // subnormal, so recompute using mantissa: c = intlog2(floor(x));
    register unsigned int t_, tt_; // temporary
    if (tt_ = x_ >> 16)
    {
        c_ = (t_ = tt_ >> 8) ? 24 + LogTable256[t_] : 16 + LogTable256[tt_];
    }
    else 
    {
        c_ = (t_ = x_ >> 8) ? 8 + LogTable256[t_] : LogTable256[x_];
    }

    return c_;
}
}; // namespace fastexp
