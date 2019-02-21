/*
 *  (C) copyright  2011, Ismael Garcia, (U.Girona/ViRVIG, Spain & INRIA/ALICE, France)
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef CONFIG_PARAMS_H_
#define CONFIG_PARAMS_H_

#include <vector>
#include <string>

#include <img_tga.h>

//------------------------------------------------------------------------
#define ENABLE_HOST_COMPUTING                0 
#define ENABLE_DEVICE_CUDA_COMPUTING         0
#define ENABLE_DEVICE_OMP_COMPUTING          1
#define ENABLE_DEVICE_OPENCL_COMPUTING       0

#define OMP_CUSTOM_OPTIONS_ON_RUNTIME        1 // Set to 1, won't take the OMP_CUSTOM_* values
                                               // Described below, but the ones set by env.
                                               // variables

#define OMP_CUSTOM_NUM_THREADS               5
#define OMP_CUSTOM_CHUNK_SIZE                256 

// export OMP_NUM_THREADS=5

// export GOMP_CPU_AFFINITY="0 1 2 3 4"        // Bind cpu thread 0 to core 0, and respectively from 1 to 4 too
// export GOMP_CPU_AFFINITY="0 3 1-2 4-15:2"   // Bind the initial thread to CPU 0, the second to CPU 3, the third 
                                               // to CPU 1, the fourth to CPU 2, the fifth to CPU 4, the sixth 
                                               // through tenth to CPUs 6, 8, 10, 12, and 14 respectively and then 
                                               // start assigning back from the beginning of the list.

// export OMP_PROC_BIND=["true" or "false"]    // true: Avoid OpenMP moves threads between processors
                                               // false: Allow OpenMP moves threads between processors

// export OMP_SCHEDULE=["static[,chunk_size]", "dynamic[,chunk_size]", "guided[,chunk_size]", or "auto"] 

// export OMP_DYNAMIC=["true" or "false"]      // OpenMP implementation may adjust the 
                                               // number of threads to use for executing 
                                               // parallel regions in order to optimize the 
                                               // use of system resources. 

// export OMP_WAIT_POLICY=["active" or "passive"] // The ACTIVE value specifies that waiting 
                                               // threads should mostly be active, consuming 
                                               // The PASSIVE value specifies that waiting 
                                               // threads should mostly be passive, not 
                                               // consuming processor cycles, while waiting. 
                                               // For example, an OpenMP implementation 
                                               // may make waiting threads yield the processor 
                                               // to other threads or go to sleep.
                                                
//------------------------------------------------------------------------
#define ENABLE_KEY_DATA_MODE                 1 // Has 64b (key+data) / 32b (key only)
#define ENABLE_2D_COORDS                     1 // Pack 2D coords as keys
#define ENABLE_3D_COORDS                     0 // Pack 3D coords as keys
#define ENABLE_LIBHU_GPU_RANDOM_GENERATOR    0 // Use faster random values generator
#define ENABLE_LIBHU_LOG                     1 // Show log of libhu functions
#define ENABLE_LIBHU_LOG_DETAILED            0 // Show log of hashed and queried keys
#define ENABLE_KERNEL_PROFILING              0 // Enable kernel profiling
#define ENABLE_GATOR                         0 // Enable gator profiling
#define ENABLE_PERF                          0 // Enable perf profiling
#define ENABLE_TRACE_CMD                     1 // Enable trace-cmd profiling
//------------------------------------------------------------------------

#if (ENABLE_TRACE_CMD)
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h> 
#endif

#if (ENABLE_DEVICE_CUDA_COMPUTING)
#define THRUST_DEVICE_BACKEND                THRUST_DEVICE_BACKEND_CUDA
#endif

#if (ENABLE_DEVICE_OMP_COMPUTING)
#define THRUST_DEVICE_BACKEND                THRUST_DEVICE_BACKEND_OMP
#endif

#if (ENABLE_DEVICE_OPENCL_COMPUTING)
#define THRUST_DEVICE_BACKEND                THRUST_DEVICE_BACKEND_OPENCL
#endif

#if (ENABLE_DEVICE_CUDA_COMPUTING)
#include <cuda.h>
#include <cuda_runtime.h>

#define DEVICE                               __device__                
#define HOST                                 __host__                
#define THRUST_VECTOR                        thrust::device_vector
#define LIBH                                 libh::detail::backend::cuda
#define LIBHU                                libhu::detail::backend::cuda
#endif

#if (ENABLE_DEVICE_OMP_COMPUTING)
#define DEVICE                               __device__                
#define HOST                                 __host__                
#define THRUST_VECTOR                        thrust::device_vector
#define LIBH                                 libh::detail::backend::omp
#define LIBHU                                libhu::detail::backend::omp
#endif

#if (ENABLE_DEVICE_OPENCL_COMPUTING)
#define __CL_ENABLE_EXCEPTIONS
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#include <CL/cl.h>
#include <CL/cl_ext.h>
#include <CL/cl.hpp>

#define DEVICE                               __device__                
#define HOST                                 __host__                
#define THRUST_VECTOR                        thrust::device_vector
#define LIBH                                 libh::detail::backend::opencl
#define LIBHU                                libhu::detail::backend::opencl
#endif

#if (ENABLE_HOST_COMPUTING)
#define DEVICE                                               
#define HOST                                 __host__
#define THRUST_VECTOR                        thrust::host_vector
#define LIBH                                 libh::detail::backend::cpp
#define LIBHU                                libhu::detail::backend::cpp
#endif

#if (ENABLE_HOST_COMPUTING || ENABLE_DEVICE_OMP_COMPUTING || ENABLE_DEVICE_OPENCL_COMPUTING)
#undef __CUDACC__
#endif

#define TIME_1G_BILLION                      1000000000.0f
#define TIME_1K_MILLISECONDS                 1000.0f

#define CLOCK_REALTIME 1

struct ConfigParams
{
  std::vector<std::string> sparams;
  std::string image_name;
  unsigned int num_keys;
  unsigned int num_extra;
  float dens;
  float rate_non_valid_keys;
  unsigned int seed;
  bool sorted_access;
  bool rand_num_mode;
  bool image_mode;
  bool access_null_keys;
  bool is_set;

  bool rh_rand_hash_state;
  bool rh_coh_hash_state;
  bool coh_hash;
  bool rand_hash;
  //--------------------------------------------
  float rh_coh_hash_build_time;
  float rh_coh_hash_access_time;
  float rh_rand_hash_build_time;
  float rh_rand_hash_access_time;
  //--------------------------------------------
  float rh_coh_hash_build_keys_per_sec;
  float rh_coh_hash_access_keys_per_sec;
  float rh_rand_hash_build_keys_per_sec;
  float rh_rand_hash_access_keys_per_sec;
  //--------------------------------------------
  unsigned int u3D_w;
  unsigned int u3D_h;
  unsigned int u3D_d;
  unsigned int u2D_w;
  unsigned int u2D_h;
  Texture *tex;
};

int testRobinHoodHash(ConfigParams& cfg);

#endif // CONFIG_PARAMS_H_
