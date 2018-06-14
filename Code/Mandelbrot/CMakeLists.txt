cmake_minimum_required(VERSION 3.3.2 FATAL_ERROR)
project(Mandelbrot CXX)

set(CMAKE_CXX_STANDARD 14) # At least 14 is required for HPX
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wno-unused-variable -pthread")

find_package(HPX REQUIRED)

set(flags COMPONENT_DEPENDENCIES iostreams)
set(sources main.cpp
        mandelbrot_set.h mandelbrot_set_hpx_coarseConsumer.cpp mandelbrot_set_hpx_fine.cpp mandelbrot_set_hpx_ultrafine.cpp mandelbrot_set_pthreads.cpp)
source_group("Source Files" FILES ${sources})

add_hpx_executable(mandelbrot
        SOURCES ${sources}
        ${flags})
