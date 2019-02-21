PROJECT(example_01)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FIND_PACKAGE(CUDA)
FIND_PACKAGE(OpenMP)

INCLUDE(FindCUDA)
SET(CUDA_SDK_ROOT_DIR $ENV{CUDA_SDK_ROOT_DIR})

set(CMAKE_TOOLCHAIN_FILE /work/cohash-read-only_win/example/toolchain.cmake)
set(MAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static") 

LINK_LIBRARIES(
#  "winmm.lib"
#  "psapi.lib"
#  "shlwapi.lib"
##  "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v5.0/lib/Win32/cudart.lib"
##  "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v5.0/lib/Win32/cuda.lib"
##"/media/ARM_DEV/local/cuda-5.0/lib64/libcudart.so"
##"/media/ARM_DEV/local/cuda-5.0/lib64/libcudart.so"
#"/usr/lib/x86_64-linux-gnu/librt.so"
#"/usr/lib/x86_64-linux-gnu/libdl.so"
#/usr/arm-linux-gnueabi/lib/librt.so
#/usr/arm-linux-gnueabi/lib/libdl.so
#/usr/arm-linux-gnueabi/lib/librt.a
#/usr/arm-linux-gnueabi/lib/libdl.a
#/media/ARM_DEV/work/dev/experimental/arndale/freeocl_arm_patched/freeocl-read-only/src/icd/libOpenCL.so
#/media/ARM_DEV/work/dev/experimental/arndale/freeocl_arm_patched/freeocl-read-only/src/libFreeOCL.so
#/media/ARM_DEV/work/dev/experimental/arndale/freeocl_x86_32/freeocl-read-only/src/icd/libOpenCL.so
#/media/ARM_DEV/work/dev/experimental/arndale/freeocl_x86_32/freeocl-read-only/src/libFreeOCL.so
#/work/freeocl_arm_patched/src/icd/libOpenCL.so
#/work/freeocl_arm_patched/src/libFreeOCL.so
)

INCLUDE_DIRECTORIES(
  "./"
  "../"
  "../include"
#  "/media/ARM_DEV/work/dev/experimental/arndale/freeocl_arm_patched/freeocl-read-only/" 
#  "/media/ARM_DEV/work/dev/experimental/arndale/freeocl_arm_patched/freeocl-read-only/include"
#  "/media/ARM_DEV/work/dev/experimental/arndale/freeocl_arm_patched/freeocl-read-only/src"
#  "/media/ARM_DEV/work/dev/experimental/arndale/freeocl_x86_32/freeocl-read-only/" 
#  "/media/ARM_DEV/work/dev/experimental/arndale/freeocl_x86_32/freeocl-read-only/include"
#  "/media/ARM_DEV/work/dev/experimental/arndale/freeocl_x86_32/freeocl-read-only/src"
#   "/work/freeocl_arm_patched/"
#   "/work/freeocl_arm_patched/include"
#   "/work/freeocl_arm_patched/src"

)

SET(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} "-arch=sm_20")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_C_FLAGS}")


if(CMAKE_COMPILER_IS_GNUCXX)
ADD_DEFINITIONS("-std=c++0x") 
endif(CMAKE_COMPILER_IS_GNUCXX)

#CUDA_ADD_EXECUTABLE(
#  example_01
#  main.cu
#  statsdb.h
#  statsdb.c
#  main.cpp
#)

ADD_EXECUTABLE(
  example_01
  main.cu
  main.cpp

  #statsdb.h
  #sqlite3.h
  #sqlite3.c 
  #statsdb.c

#  os/perf_event.h
#  os/pmf_perf_counter.h
#  os/pmf_perf_counter.cpp
)

INSTALL(TARGETS example_01 CONFIGURATIONS Release None "" DESTINATION ${example_01_SOURCE_DIR})
