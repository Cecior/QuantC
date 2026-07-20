cmake_minimum_required(VERSION 4.2)
project(QuantC LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 17)

include_directories(.)
find_package(OpenMP REQUIRED)

add_executable(QuantC
        kernel_cpu.h
        kernel_cpu.cpp
        main.cu
)

if(OpenMP_CXX_FOUND)
    target_link_libraries(QuantC PUBLIC OpenMP::OpenMP_CXX)
endif()

if(MSVC)
    target_compile_options(QuantC PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>: /O2 /arch:AVX2 /fp:fast /Zi>
        $<$<COMPILE_LANGUAGE:CUDA>: -O3 --use_fast_math -lineinfo>
    )
else()
    target_compile_options(QuantC PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>: -O3 -march=native -ffast-math -flto -mavx2 -g>
        $<$<COMPILE_LANGUAGE:CUDA>: -O3 --use_fast_math -lineinfo>
    )
endif()