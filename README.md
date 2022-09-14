# Fast linear exp

Simple fast exp using SIMD intrinsics

## Requirements

- SSSE3 support
- C++20 compliant compiler (used for LUT generation)

## Fetching files
The process is slightly varies from default, as this package uses external library gcem for generating LUT
```
git clone https://github.com/juviee/smartengines_testcase
git submodule update --init
```
## Building command
```
c++ test_funcs.cpp fast_exp.cpp -ggdb3 -std=c++20 -DDEBUG_V -Wextra -mSSSE3 -Igcem/include -o test
```
Probably you want to change 'c++' part with your favorite compiler
## Running
```
./test
```