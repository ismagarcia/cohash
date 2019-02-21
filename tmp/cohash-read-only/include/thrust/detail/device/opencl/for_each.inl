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


/*! \file for_each.inl
 *  \brief Inline file for for_each.h.
 */

#include <limits>

#include <thrust/detail/config.h>
#include <thrust/detail/device/dereference.h>
#include <thrust/detail/device/opencl/launch_closure.h>
#include <thrust/detail/static_assert.h>

#include <CL/cl.h>

extern int get_local_id (int);
extern int get_local_size (int);
extern int get_group_id (int);
extern int get_num_groups (int);

namespace thrust
{
namespace detail
{
namespace device
{
namespace opencl
{

template<typename RandomAccessIterator,
         typename Size,
         typename UnaryFunction>
  struct for_each_n_closure
{
  typedef void result_type;

  RandomAccessIterator first;
  Size n;
  UnaryFunction f;

  for_each_n_closure(RandomAccessIterator first_,
                     Size n_,
                     UnaryFunction f_)
    : first(first_),
      n(n_),
      f(f_)
  {}

// OpenCL built-in variables
  __device__
  result_type operator()(void)
  {
    const Size grid_size = get_local_size(0) * get_num_groups(0);

    Size i = get_group_id(0) * get_local_size(0) + get_local_id(0);

    // advance iterator
    first += i;

    while(i < n)
    {
      f(thrust::detail::device::dereference(first));
      i += grid_size;
      first += grid_size;
    }
  }
};


template<typename RandomAccessIterator,
         typename Size,
         typename UnaryFunction>
RandomAccessIterator for_each_n(RandomAccessIterator first,
                                Size n,
                                UnaryFunction f)
{
  if (n <= 0) return first;  //empty range
 
  if ((sizeof(Size) > sizeof(unsigned int))
       && n > Size((std::numeric_limits<unsigned int>::max)())) // convert to Size to avoid a warning
  {
    // n is large, must use 64-bit indices
    typedef for_each_n_closure<RandomAccessIterator, Size, UnaryFunction> Closure;
    Closure closure(first, n, f);
    launch_closure(closure, n);
  }
  else
  {
    // n is small, 32-bit indices are sufficient
    typedef for_each_n_closure<RandomAccessIterator, unsigned int, UnaryFunction> Closure;
    Closure closure(first, static_cast<unsigned int>(n), f);
    launch_closure(closure, static_cast<unsigned int>(n));
  }

  return first + n;
} 


} // end namespace opencl
} // end namespace device
} // end namespace detail
} // end namespace thrust

