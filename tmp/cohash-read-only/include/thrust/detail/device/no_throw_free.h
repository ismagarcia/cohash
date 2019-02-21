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

#pragma once

#include <thrust/detail/config.h>
#include <thrust/device_ptr.h>
#include <thrust/iterator/iterator_traits.h>
#include <thrust/detail/device/dispatch/no_throw_free.h>

namespace thrust
{

namespace detail
{

namespace device
{


inline void no_throw_free(thrust::device_ptr<void> ptr) throw()
{
  // IG 09/12/2012 ismael.garcia@arm.com
  //typedef thrust::iterator_space< thrust::device_ptr<void> >::type Space;
  //thrust::detail::device::dispatch::no_throw_free<0>(ptr, Space());
} // end no_throw_


} // end device

} // end detail

} // end thrust

