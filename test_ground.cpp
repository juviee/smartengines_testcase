#include <immintrin.h>
#include <iostream>

int main()
{
    float arr[] = {0, 1, 2, 3};

    float e4arr[] = { 10, 11, 12, 13};
    __m128 e4 = _mm_set_ps( 10, 11, 12, 13 ); 

    __m128d ll = _mm_loadl_pd( _mm_castps_pd(e4), (double*) arr );
    __m128d lh = _mm_loadh_pd( _mm_castps_pd(e4), (double*) arr );

    float out_loadl[] = {0, 0, 0, 0};
    float out_loadh[] = {0, 0, 0, 0};

    _mm_store_pd((double*)out_loadl, ll);
    _mm_store_pd((double*)out_loadh, lh);

    std::cout << "data arr: [";

    for(auto &i : arr)
        std::cout << i << " | ";
    std::cout << "] \n";
    
    std::cout << "on reg e4: [";

    for(auto &i : e4arr)
        std::cout << i << " | ";
    std::cout << "] \n";

    std::cout << "on loadl(e4, data_arr): [";
    for(auto &i : out_loadl)
        std::cout << i << " | ";
    std::cout << "] \n";

    std::cout << "on loadh(e4, data_arr): [";
    for(auto &i : out_loadh)
        std::cout << i << " | ";
    std::cout << "] \n";

// cout
//data arr: [0 | 1 | 2 | 3 | ]
//on reg e4: [10 | 11 | 12 | 13 | ]
//on loadl(e4, data_arr): [0 | 1 | 11 | 10 | ]
//on loadh(e4, data_arr): [13 | 12 | 0 | 1 | ]
}
