#ifndef LUT_HPP
#define LUT_HPP

#include <array>
namespace fastexp
{
template <std::size_t N, typename F = std::identity>
constexpr auto gen_float_array_exp_step(const F& f = F{}) 
{
   std::array<float, N> arr;
   for (size_t i = 0; i < N; ++i)
      arr[i] = f(static_cast<size_t>(1<<i));
   return arr;
}
}; // namespace fastexp
#endif
