cmake_minimum_required(VERSION 3.2)
project(QuantC LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(${CMAKE_SOURCE_DIR}/include)

enable_language(CUDA)

find_package(OpenMP REQUIRED)

include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY  https://www.github.com/pybind/pybind11.git
    GIT_TAG         v2.11.1
)
FetchContent_MakeAvailable(pybind11)

add_library(backend_naive STATIC src/naive.cpp)
target_compile_options(backend_naive PRIVATE -O3 -fno-tree-vectorize)

add_library(backend_omp STATIC src/omp.cpp)
target_compile_options(backend_omp PRIVATE -O3)
target_link_libraries(backend_omp PRIVATE OpenMP::OpenMP_CXX)

add_library(backend_avx STATIC src/avx.cpp)
target_compile_options(backend_avx PRIVATE -O3 -march=native)

add_library(backend_cuda STATIC src/cuda.cu)
target_compile_options(backend_cuda PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-O3 -arch=sm_86>)