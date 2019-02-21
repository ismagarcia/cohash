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

#if THRUST_DEVICE_BACKEND == THRUST_DEVICE_BACKEND_OPENCL

#include <thrust/detail/device/opencl/malloc.h>
#include <thrust/device_ptr.h>
#include <stdexcept>

namespace thrust
{

namespace detail
{

namespace device
{

namespace opencl
{

template<unsigned int DummyParameterToPreventInstantiation>
thrust::device_ptr<void> malloc(const std::size_t n)
{

  // IG 09/12/2012 ismael.garcia@arm.com
  cl_mem result;
  cl_int error;

  cl_platform_id platforms[2];
  cl_uint platforms_n = 0;
  clGetPlatformIDs(2, platforms, &platforms_n);
  cl_device_id devices[2];
  cl_uint devices_n = 0;
  //clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 2, devices, &devices_n));
  cl_context context;
  //context = clCreateContext(NULL, 1, devices, &pfn_notify, NULL, &error);

  //result = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(void)*n, NULL, &error));
  /*
  if(error)
  {
    throw std::bad_alloc();
  /} // end if
  return thrust::device_ptr<void>(result);
  */
  // IG 09/12/2012 ismael.garcia@arm.com
  thrust::device_ptr<void> p;
  return p;
} // end malloc()

} // end namespace opencl

} // end namespace device

} // end namespace detail

} // end namespace thrust

#endif // THRUST_DEVICE_BACKEND_OPENCL

