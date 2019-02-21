/*
 *  Copyright 2008-2010 NVIDIA Corporation
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

#include <thrust/detail/config.h>

// do not attempt to compile this file, which relies on 
// CUDART without system support
#if THRUST_DEVICE_BACKEND == THRUST_DEVICE_BACKEND_OPENCL

#include <thrust/detail/device/opencl/free.h>
#include <CL/cl.h>
#include <thrust/system_error.h>

namespace thrust
{
namespace detail
{
namespace device
{
namespace opencl
{

template<unsigned int DummyParameterToAvoidInstantiation>
void free(thrust::device_ptr<void> ptr)
{
  //cl_int error = clReleaseMemObject(ptr.get());  
  //if(error)
  //{
  //  throw thrust::system_error(error, thrust::opencl_category());
  //} // end error
} // end free()

} // end namespace opencl
} // end namespace device
} // end namespace detail
} // end namespace thrust

#endif // THRUST_DEVICE_BACKEND

