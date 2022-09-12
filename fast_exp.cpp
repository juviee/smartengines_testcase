#include "fast_exp.hpp"
#include <iostream>
#include <cmath>
#include <array>

namespace fastexp
{
int main()
{
    return 0;
}
// ***********
// fastexp sse
// ***********
void fast_exp_sse(float* in, float* out, size_t size)
{
    // (e ^ x) = lut_c[~x] + lut_exp[~x] * x
    // where lut_c[] and lut_exp[] are compile_time calculated
    // ~x -- highest byte in x(at int form, MSB)
    size_t pos = 0;
    for(; pos < size; pos+=4)
        // let num = 9
        //
    {
        fast_exp_sse_n<4>(in+pos, out+pos); // filling out w/ sse func 
    }
    pos -= 4;
    fast_exp_sse_n<(size - 1) - pos>(in+pos, out+pos);
    // that will call one of fast_exp_sse_n<0..3>
}

// **************************************
// template vectorized fast_exp using sse
// **************************************
template<size_t n>
void fast_exp_sse_n>(float* in, float* out)
{
// dummy for size_t n tmplt
// use specifications!
    return;
}
template<>
void fast_exp_sse_n<4>(float* in, float* out)
{
    uint32_t msb[4];
    get_msb_sse_n<4>(in, msb);
    __m128 c4 = _mm_set_ps( lut_c[msb[0]], 
                            lut_c[msb[1]],
                            lut_c[msb[2]],
                            lut_c[msb[3]]); // y offset at msb point

    __m128 e4 = _mm_set_ps( lut_exp[msb[0]], 
                            lut_exp[msb[1]],
                            lut_exp[msb[2]],
                            lut_exp[msb[3]]); // exp at msb point

    __m128 x4 = _mm_load_ps( in ); // x_pos from (in)
                                   // m8 be better using stream_load
     e4 = _mm_mul_ps( e4, x4 ); // lut_exp(~x) * x (broadcasting, not dot)
     c4 = _mm_add_ps( e4, c4 ); // lut_c(~x) + lut_exp(~x) * x
     _mm_store_ps( out );
}

template<>
void fast_exp_sse_n<3>(float* in, float* out)
{
    uint32_t msb[4];
    get_msb_sse_n<3>(in, msb);
    __m128 c4 = _mm_set_ps( lut_c[msb[0]], 
                            lut_c[msb[1]],
                            lut_c[msb[2]],
                            0); // y offset at msb point

    __m128 e4 = _mm_set_ps( lut_exp[msb[0]], 
                            lut_exp[msb[1]],
                            lut_exp[msb[2]],
                            in[2]); // exp at msb point

    __m128 x4 = _mm_castpd_ps(
                _mm_loadl_pd( _mm_castps_pd(e4), (double*) in )
                );

//  x4 = [ in[0] : in[1] : e4[3] == in[2] : e4[2] ]
//
//    const __m128i swap_3_4_mask = __mm_set_epi8(
//        15, 14, 13, 12, 11, 10, 9, 8, 3, 2, 1, 0, 7, 6, 5, 4
//    );

    
    // we use set instead of load because possible memcorruption
    // as alternative, we possibly could use masked_load but this
    // requires AVX support, which is much more platform dependent

     e4 = _mm_mul_ps( e4, x4 ); // lut_exp(~x) * x (broadcasting, not dot)
     c4 = _mm_add_ps( e4, c4 ); // lut_c(~x) + lut_exp(~x) * x
     _mm_store_ps( out, c4 );

}

template<>
void fast_exp_sse_n<2>(float* in, float* out)
{
    uint32_t msb[4];
    get_msb_sse_n<3>(in, msb);
    __m128 c4 = _mm_set_ps( lut_c[msb[0]], 
                            lut_c[msb[1]],
                            0,
                            0); // y offset at msb point

    __m128 e4 = _mm_set_ps( lut_exp[msb[0]], 
                            lut_exp[msb[1]],
                            0,
                            0); // exp at msb point

    __m128 x4 = _mm_loadh_pd( (double*) in, e4 );
    //e4 is dummy
                    
                    
                    

}

template<>
void fast_exp_sse_n<1>(float* in, float* out)
{

}

void fast_exp_sse_n<0>(float* in, float* out)
{

}

// *********************
// MSB template function 
// *********************
// additional_memory......
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
    
}

template<>
void get_msb_sse_n<3>(float* in, int32_t* out)
{
    
}

template<>
void get_msb_sse_n<2>(float* in, int32_t* out)
{
    
}

template<>
void get_msb_sse_n<1>(float* in, int32_t* out)
{

}

template<>
void get_msb_sse_n<0>(float* in, int32_t* out)
{
    return;
}
}; // namespace fastexp
