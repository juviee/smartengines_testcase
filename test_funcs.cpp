#include "fast_exp.hpp"
#include <iostream>
#include <cmath>

#include <xmmintrin.h>
namespace fastexp
{
namespace test
{
template <typename t>
void print_arr_n( t arr[], std::string prefix, size_t n )
{
    std::cout << prefix << ": [";
    for(size_t i = 0; i < n; ++i)
        std::cout << arr[i] << " | ";
    std::cout << "] \n";
}
void test_get_msb_4()
{
    std::cout << "========================" << std::endl;
    std::cout << "TEST GET MSB 4" << std::endl;
    std::cout << "========================" << std::endl;
    int32_t dst[4];
    float src[]{ 4.5, 63.8, 32.777, 15.999};
    get_msb_sse_n<4>(src, dst);
    print_arr_n(src, "Input arr", 4);
    print_arr_n(dst, "get_msb_4 arr", 4);
}

void test_get_msb_4_lval()
{
    std::cout << "========================" << std::endl;
    std::cout << "TEST GET MSB 4 with lowval" << std::endl;
    std::cout << "========================" << std::endl;
    int32_t dst[4];
    float src[]{ -1, 0.1, 1, 1.5};
    get_msb_sse_n<4>(src, dst);
    print_arr_n(src, "Input arr", 4);
    print_arr_n(dst, "get_msb_4 arr", 4);
}
void test_get_msb_3()
{
    std::cout << "========================" << std::endl;
    std::cout << "TEST GET MSB 3" << std::endl;
    std::cout << "========================" << std::endl;
    int32_t dst[3];
    float src[]{ 4.5, 63.8, 32.777};
    get_msb_sse_n<3>(src, dst);
    print_arr_n(src, "Input arr", 3);
    print_arr_n(dst, "get_msb_3 arr", 3);
}

void test_get_msb_2()
{
    std::cout << "========================" << std::endl;
    std::cout << "TEST GET MSB 2" << std::endl;
    std::cout << "========================" << std::endl;
    int32_t dst[2];
    float src[]{ 4.5, 63.8};
    get_msb_sse_n<2>(src, dst);
    print_arr_n(src, "Input arr", 2);
    print_arr_n(dst, "get_msb_3 arr", 2);
}
void test_get_msb_1()
{
    std::cout << "========================" << std::endl;
    std::cout << "TEST GET MSB 1" << std::endl;
    std::cout << "========================" << std::endl;
    int32_t dst[1];
    float src[]{ 4.5, 63.8};
    get_msb_sse_n<1>(src, dst);
    print_arr_n(src, "Input arr", 1);
    print_arr_n(dst, "get_msb_3 arr", 1);
}

void precise_exp(float* src, float* dst, size_t n)
{
    for(size_t i = 0; i < n; ++i)
    {
        dst[i] = std::exp(src[i]);
    }
}

void test_fast_exp_4()
{
    std::cout << "========================" << std::endl;
    std::cout << "TEST FAST EXP 4" << std::endl;
    std::cout << "========================" << std::endl;
    float dst[4];
    float src[]{ 1, 1.5, 2.7, 15.999};
    fast_exp_sse_n<4>(src, dst);
    print_arr_n(src, "Input arr", 4);
    print_arr_n(dst, "fast_exp_4 arr", 4);

    
    int32_t msb[4];
    get_msb_sse_n<4>(src, msb);
    print_arr_n(msb, "get_msb_sse_n<4> arr", 4);

    float precise[4];
    precise_exp(src, precise, 4);
    print_arr_n(precise, "precise_exp arr", 4);
    std::cout << "========================" << std::endl;
}

void test_fast_exp_3()
{
    std::cout << "========================" << std::endl;
    std::cout << "TEST FAST EXP 3" << std::endl;
    std::cout << "========================" << std::endl;
    float dst[3];
    float src[]{1, 1.5, 2.7};
    fast_exp_sse_n<3>(src, dst);
    print_arr_n(src, "Input arr", 3);
    print_arr_n(dst, "fast_exp_3 arr", 3);

    
    int32_t msb[3];
    get_msb_sse_n<3>(src, msb);
    print_arr_n(msb, "get_msb_sse_n<3> arr", 3);

    float precise[3];
    precise_exp(src, precise, 3);
    print_arr_n(precise, "precise_exp arr", 3);
    std::cout << "========================" << std::endl;
}

void test_fast_exp_2()
{
    std::cout << "========================" << std::endl;
    std::cout << "TEST FAST EXP 2" << std::endl;
    std::cout << "========================" << std::endl;
    float dst[2];
    float src[]{ 1.2, 3.5};
    fast_exp_sse_n<2>(src, dst);
    print_arr_n(src, "Input arr", 2);
    print_arr_n(dst, "fast_exp_2 arr", 2);

    
    int32_t msb[2];
    get_msb_sse_n<2>(src, msb);
    print_arr_n(msb, "get_msb_sse_n<2> arr", 2);

    float precise[2];
    precise_exp(src, precise, 2);
    print_arr_n(precise, "precise_exp arr", 2);
    std::cout << "========================" << std::endl;
}
void test_fast_exp_1()
{
    std::cout << "========================" << std::endl;
    std::cout << "TEST FAST EXP 1" << std::endl;
    std::cout << "========================" << std::endl;
    float dst[1];
    float src[]{ 1.2 };
    fast_exp_sse_n<1>(src, dst);
    print_arr_n(src, "Input arr", 1);
    print_arr_n(dst, "fast_exp_1 arr", 1);

    
    int32_t msb[1];
    get_msb_sse_n<1>(src, msb);
    print_arr_n(msb, "get_msb_sse_n<1> arr", 1);

    float precise[1];
    precise_exp(src, precise, 1);
    print_arr_n(precise, "precise_exp arr", 1);
    std::cout << "========================" << std::endl;
}
void test_fast_exp_4_low_val()
{
    std::cout << "========================" << std::endl;
    std::cout << "TEST FAST EXP 4 with low val" << std::endl;
    std::cout << "========================" << std::endl;
    float dst[4];
    float src[]{ -2.2, -1.3, 0, 0.7};
    fast_exp_sse_n<4>(src, dst);
    print_arr_n(src, "Input arr", 4);
    print_arr_n(dst, "fast_exp_4 arr", 4);

    
    int32_t msb[4];
    get_msb_sse_n<4>(src, msb);
    print_arr_n(msb, "get_msb_sse_n<4> arr", 4);

    float precise[4];
    precise_exp(src, precise, 4);
    print_arr_n(precise, "precise_exp arr", 4);
    std::cout << "========================" << std::endl;
}

void test_fast_exp_iterative()
{
    std::cout << "========================" << std::endl;
    std::cout << "TEST FAST EXP with iterative" << std::endl;
    std::cout << "========================" << std::endl;
    size_t size = 10;
    float* src = new float[size];
    float atzero = -1; float incr = 0.3;
    for(size_t i = 0; i < size; ++i)
    {
        src[i] = atzero + i * incr;
    }
    float* dst = new float[size];
    fast_exp_sse(src, dst, size);
    print_arr_n(src, "Input arr", size);
    print_arr_n(dst, "fast_exp_sse arr", size);

    precise_exp(src, dst, size);
    print_arr_n(dst, "precise_exp arr", size);
    std::cout << "========================" << std::endl;

    delete[] src;
    delete[] dst;
}
}//namespace test
}//namespace fastexp
int main()
{
    using namespace fastexp::test;
    test_get_msb_4();
    test_get_msb_3();
    test_get_msb_2();
    test_get_msb_1();

    test_get_msb_4_lval();
    test_fast_exp_4();
    test_fast_exp_3();
    test_fast_exp_2();
    test_fast_exp_1();
    
    test_fast_exp_4_low_val();

    test_fast_exp_iterative();
}
