# References:
#   - https://software.intel.com/en-us/articles/iso-iec-standards-language-conformance-for-intel-c-compiler
#   - https://software.intel.com/en-us/articles/c99-support-in-intel-c-compiler
#   - https://software.intel.com/en-us/articles/c11-support-in-intel-c-compiler

set(_cmake_oldestSupported "__ICC >= 1110")

set(DETECT_C99 "defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L")
set(DETECT_C11 "defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201000L")

set(Intel_C90 "${_cmake_oldestSupported}")
set(Intel_C99 "${_cmake_oldestSupported} && ${DETECT_C99}")
set(Intel_C11 "__ICC >= 1500 && ${DETECT_C11}")


set(_cmake_feature_test_c_static_assert "${Intel_C11}")

set(_cmake_feature_test_c_restrict "${Intel_C99}")
set(_cmake_feature_test_c_variadic_macros "${Intel_C99}")
set(_cmake_feature_test_c_function_prototypes "${Intel_C90}")