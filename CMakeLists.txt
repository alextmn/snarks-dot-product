cmake_minimum_required (VERSION 2.8.12)
add_compile_options(-std=c++11)

project (SnarksDotProduct)
# The version number.
set (SnarksDotProduct_VERSION_MAJOR 1)
set (SnarksDotProduct_VERSION_MINOR 0)
set (CURVE "BN128")
set (LIBSNARK  "${PROJECT_SOURCE_DIR}/../libsnark")
 
add_definitions(
  -DCURVE_${CURVE}
)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/MainConfig.h.in"
  "${PROJECT_BINARY_DIR}/MainConfig.h"
  )
 
# add the binary tree to the search path for include files
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${LIBSNARK}")
include_directories("${LIBSNARK}/depends/libff")
include_directories("${LIBSNARK}/depends/libfqfft")

# add the executable
add_executable(SnarksDotProduct "${PROJECT_SOURCE_DIR}/dotproduct_r1cs_ppzksnark.cpp")

target_link_libraries(SnarksDotProduct  gmp gmpxx crypto)
target_link_libraries(SnarksDotProduct "${LIBSNARK}/build/depends/libzm.a")
target_link_libraries(SnarksDotProduct "${LIBSNARK}/build/depends/libff/libff/libff.a")
target_link_libraries(SnarksDotProduct "${LIBSNARK}/build/libsnark/libsnark.a")

#target_link_libraries(SnarksDotProduct  /usr/local/lib/libgmp.dylib)
#target_link_libraries(SnarksDotProduct  /usr/local/lib/libgmpxx.dylib)

#/usr/local/lib/libgmpxx.dylib

