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


/*! \file opencl_error.h
 */

#pragma once

#include <thrust/detail/config.h>
#include <thrust/detail/type_traits.h>
#include <thrust/system/error_code.h>
#include <driver_types.h>

namespace thrust
{

namespace system
{

/*! \addtogroup system
 *  \{
 */

// To construct an error_code after a opencl Runtime error:
//
//   error_code(::openclGetLastError(), opencl_category())

// XXX N3000 prefers enum class opencl_errc { ... }
namespace opencl_errc
{

enum opencl_errc_t
{
  // from opencl/include/driver_types.h
  success                      ,//= openclSuccess,
  missing_configuration        ,//MissingConfiguration,
  memory_allocation            ,//MemoryAllocation,
  initialization_error         ,//InitializationError,
  launch_failure               ,//LaunchFailure,
  prior_launch_failure         ,//PriorLaunchFailure,
  launch_timeout               ,//LaunchTimeout,
  launch_out_of_resources      ,//LaunchOutOfResources,
  invalid_device_function      ,//InvalidDeviceFunction,
  invalid_configuration        ,//InvalidConfiguration,
  invalid_device               ,//InvalidDevice,
  invalid_value                ,//InvalidValue,
  invalid_pitch_value          ,//InvalidPitchValue,
  invalid_symbol               ,//InvalidSymbol,
  map_buffer_object_failed     ,//MapBufferObjectFailed,
  unmap_buffer_object_failed   ,//UnmapBufferObjectFailed,
  invalid_host_pointer         ,//InvalidHostPointer,
  invalid_device_pointer       ,//InvalidDevicePointer,
  invalid_texture              ,//InvalidTexture,
  invalid_texture_binding      ,//InvalidTextureBinding,
  invalid_channel_descriptor   ,//InvalidChannelDescriptor,
  invalid_memcpy_direction     ,//InvalidMemcpyDirection,
  address_of_constant_error    ,//AddressOfConstant,
  texture_fetch_failed         ,//TextureFetchFailed,
  texture_not_bound            ,//TextureNotBound,
  synchronization_error        ,//SynchronizationError,
  invalid_filter_setting       ,//InvalidFilterSetting,
  invalid_norm_setting         ,//InvalidNormSetting,
  mixed_device_execution       ,//MixedDeviceExecution,
  opencl_runtime_unloading       ,//openclrtUnloading,
  unknown                      ,//Unknown,
  not_yet_implemented          ,//NotYetImplemented,
  memory_value_too_large       ,//MemoryValueTooLarge,
  invalid_resource_handle      ,//InvalidResourceHandle,
  not_ready                    ,//NotReady,
  insufficient_driver          ,//InsufficientDriver,
  set_on_active_process_error  ,//SetOnActiveProcess,
  no_device                    ,//NoDevice,
  ecc_uncorrectable            ,//ECCUncorrectable,
  startup_failure              ,//StartupFailure,
}; // end opencl_errc_t


} // end namespace opencl_errc


// XXX N3000 prefers is_error_code_enum<opencl_errc>
template<> struct is_error_code_enum<opencl_errc::opencl_errc_t> : thrust::detail::true_type {};


// XXX replace opencl_errc::opencl_errc_t with opencl_errc upon c++0x
/*! \return <tt>error_code(static_cast<int>(e), opencl_category())</tt>
 */
inline error_code make_error_code(opencl_errc::opencl_errc_t e);


// XXX replace opencl_errc::opencl_errc_t with opencl_error upon c++0x
/*! \return <tt>error_condition(static_cast<int>(e), opencl_category())</tt>.
 */
inline error_condition make_error_condition(opencl_errc::opencl_errc_t e);


/*! \return A reference to an object of a type derived from class \p error_category.
 *  \note The object's \p equivalent virtual functions shall behave as specified
 *        for the class \p error_category. The object's \p name virtual function shall
 *        return a pointer to the string <tt>"opencl"</tt>. The object's
 *        \p default_error_condition virtual function shall behave as follows:
 *
 *        If the argument <tt>ev</tt> corresponds to a opencl error value, the function
 *        shall return <tt>error_condition(ev,opencl_category())</tt>.
 *        Otherwise, the function shall return <tt>system_category.default_error_condition(ev)</tt>.
 */
inline const error_category &opencl_category(void);

/*! \} // end system
 */


} // end system

using system::opencl_category;

// XXX replace with using system::opencl_errc upon c++0x
namespace opencl_errc = system::opencl_errc;

} // end namespace thrust

#include <thrust/system/detail/opencl_error.inl>

