# Fast linear exp

Simple fast exp using SIMD intrinsics

## Requirements

- SSSE3 support
- C++20 compliant compiler (used for LUT generation)

## Building command
```
c++ test_funcs.cpp fast_exp.cpp -ggdb3 -std=c++20 -DDEBUG_V -Wextra -mSSSE3 -Igcem/include -o test
```
Probably you want to change 'c++' part with your favorite compiler
## Running
```
./test
```