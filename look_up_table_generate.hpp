#ifndef LUT_HPP
#define LUT_HPP

#include <array>
namespace fastexp
{
template <std::size_t N, typename F = std::identity>
constexpr auto gen_float_array_exp_step(const F& f = F{}) 
{
    std::array<float, N+1> arr;
    arr[0] = f(static_cast<size_t>(0));
    for (size_t i = 0; i < N; ++i)
    {
        arr[i+1] = f(static_cast<size_t>(1<<i));
    }
    return arr;
}

template <std::size_t N, typename F = std::identity>
constexpr auto gen_float_arr_lookup_exp_step(const F& f = F{}) 
{
    std::array<float, N+1> arr;
    arr[0] = f(static_cast<size_t>(0), static_cast<size_t>(1));
    for (size_t i = 0; i < N; ++i)
    {
        arr[i+1] = f(static_cast<int64_t>(1<<i),
                     static_cast<int64_t>(1<<(i+1))
                    );
    }
    return arr;
}
}; // namespace fastexp
#endif
