/* Copyright:
 * ----------------------------------------------------------------------------
 * This confidential and proprietary software may be used only as authorized
 * by a licensing agreement from ARM Limited.
 *      (C) COPYRIGHT 2012 ARM Limited, ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorized copies and
 * copies may only be made to the extent permitted by a licensing agreement
 * from ARM Limited.
 * ----------------------------------------------------------------------------
 */

/******************************************************************************
* Include files
******************************************************************************/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>

#include <sys/stat.h>

#include "mcl_utils.h"

/******************************************************************************
* Load the program source from disk
******************************************************************************/
const char * 
mcl_load_program_source (
	const char *file_abspath
)
{ 
	struct stat statbuf;
	
	FILE *fh = fopen(file_abspath, "r");
	if (NULL == fh)
	{
		return NULL; 
	}
	
	stat(file_abspath, &statbuf);
	char *source = (char *) malloc(statbuf.st_size + 1);
	fread(source, statbuf.st_size, 1, fh);
	source[statbuf.st_size] = '\0'; 
	
	return source; 
}

/******************************************************************************
* Release program source
******************************************************************************/
void 
mcl_release_program_source (
	char *source
)
{ 
	if (NULL != source)
	{
		free(source);
	}
}

/******************************************************************************
* Load the program binary from disk
******************************************************************************/
const unsigned char * 
mcl_load_program_binary (
	const char *file_abspath,
	struct stat *statbuf
)
{ 
	FILE *fh = fopen(file_abspath, "rb");
	if (NULL == fh)
	{
		return NULL; 
	}
	
	stat(file_abspath, statbuf);
	unsigned char *binary = (unsigned char *) malloc(statbuf->st_size);
	fread(binary, statbuf->st_size, 1, fh);
	
	return binary; 
}

/******************************************************************************
* Release program binary
******************************************************************************/
void 
mcl_release_program_binary (
	char *binary
)
{ 
	if (NULL != binary)
	{
		free(binary);
	}
}

/******************************************************************************
* Create Device Information, Context and Command Queue
******************************************************************************/
mcl_state_t * 
mcl_create_state (
	cl_int *err
)
{
	*err = CL_SUCCESS;
	
	mcl_state_t *state = (mcl_state_t *) malloc(sizeof(mcl_state_t));
	if (NULL == state)
	{
		fprintf(stderr, "Error: Failed to allocate memory");
		*err = CL_OUT_OF_HOST_MEMORY;
		return NULL;
	}
	else
	{
		memset(state, 0, sizeof(mcl_state_t));
	}
	
	/*
	 * Context and Command Queue
	 */
	{
		/* Find a GPU device */
		*err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &(state->device), NULL);
		assert(CL_SUCCESS == *err);
		
		/* Create a context to work with the device */
		state->context = clCreateContext(NULL, 1, &(state->device), NULL, NULL, err);
		assert(CL_SUCCESS == *err);
		
		/* Create a command queue for the context */
		state->queue = clCreateCommandQueue(state->context, state->device, 
		                                    CL_QUEUE_PROFILING_ENABLE, NULL);
	}
	
	/*
	 * Device Information
	 */
	{
		cl_char vendor_name[1024] = {0};
		cl_char device_name[1024] = {0};
		
		/* Get information about the device */
		*err = clGetDeviceInfo(state->device, CL_DEVICE_VENDOR,
								sizeof(vendor_name), vendor_name, NULL);
		*err |= clGetDeviceInfo(state->device, CL_DEVICE_NAME,
								sizeof(device_name), device_name, NULL);
		assert(CL_SUCCESS == *err);
		
		cl_uint clock_freq_mhz = 0;
		*err = clGetDeviceInfo(state->device, CL_DEVICE_MAX_CLOCK_FREQUENCY, 
								sizeof(clock_freq_mhz), &clock_freq_mhz, NULL);
		assert(CL_SUCCESS == *err);
	}
	return state;
}

/******************************************************************************
* Create Device Information, Context and Command Queue with device type/name
******************************************************************************/
mcl_state_t *
mcl_create_state (
	cl_int *err,
	const char *device_type_or_name
)
{
	*err = CL_SUCCESS;

	mcl_state_t *state = (mcl_state_t *) malloc(sizeof(mcl_state_t));
	if (NULL == state)
	{
		fprintf(stderr, "Error: Failed to allocate memory");
		*err = CL_OUT_OF_HOST_MEMORY;
		return NULL;
	}
	else
	{
		memset(state, 0, sizeof(mcl_state_t));
	}

	/*
	 * Context and Command Queue
	 */
	{
		cl_device_type device_type = CL_DEVICE_TYPE_DEFAULT;
		std::string device_str = device_type_or_name;
		bool all_devices = false;
		bool match_device_name = false;

		if(device_str == "cpu")
			device_type = CL_DEVICE_TYPE_CPU;
		else if(device_str == "gpu")
			device_type = CL_DEVICE_TYPE_GPU;
		else if(device_str == "accelerator")
			device_type = CL_DEVICE_TYPE_ACCELERATOR;
		else if(device_str == "default")
			device_type = CL_DEVICE_TYPE_DEFAULT;
		else if(device_str == "all" || device_str == "any")
			device_type = CL_DEVICE_TYPE_ALL;
		else if(device_str == "list")
		{
			device_type = CL_DEVICE_TYPE_ALL;
			all_devices = true;
		}
		else if(!device_str.empty())
		{
			device_type = CL_DEVICE_TYPE_ALL;
			all_devices = true;
			match_device_name = true;
		}

		/* Find a device */
		if(all_devices)
		{
			cl_device_id device_ids[32];
			cl_uint device_count = 0;

			cl_int result = clGetDeviceIDs(NULL, device_type, 32, device_ids, &device_count);
			if(CL_SUCCESS != result)
			{
				*err = result;
			}
			else
			{
				bool found_device = false;

				for(cl_uint i = 0; i < device_count; ++i)
				{
					cl_char device_name[1024] = {0};

					result = clGetDeviceInfo(device_ids[i], CL_DEVICE_NAME,
												sizeof(device_name), device_name, NULL);
					if(CL_SUCCESS == result)
					{
						if(match_device_name)
						{
							if(device_str == (char*)device_name)
							{
								state->device = device_ids[i];
								found_device = true;

								*err = CL_SUCCESS;

								break;
							}
						}
						else
						{
							cl_device_type type;
							result = clGetDeviceInfo(device_ids[i], CL_DEVICE_TYPE, sizeof(type), &type, NULL);
							if(CL_SUCCESS == result)
							{
								const char *type_str = "Unknown device";
								switch(type)
								{
									case CL_DEVICE_TYPE_CPU:
										type_str = "CPU device";
										break;
									case CL_DEVICE_TYPE_GPU:
										type_str = "GPU device";
										break;
									case CL_DEVICE_TYPE_ACCELERATOR:
										type_str = "Accelerator device";
										break;
									case CL_DEVICE_TYPE_DEFAULT:
										type_str = "Default device";
										break;
								}

								std::cout << type_str << ": " << device_name << std::endl;
							}
						}
					}
				}

				if(!found_device)
					*err = CL_DEVICE_NOT_FOUND;
			}
		}
		else
		{
      //#####################################################################
      // IG 04022013 ismael.garcia@arm.com
#ifdef FREEOCL      
      char deviceName[256];
      cl_uint numPlatforms;
      cl_uint numDevices;
      cl_device_id devices[1];
      cl_platform_id platforms[1];
      clGetPlatformIDs(0, NULL, &numPlatforms);
      clGetPlatformIDs(0, platforms, NULL);
      //device_type = CL_DEVICE_TYPE_CPU;
      clGetDeviceIDs(platforms[0], device_type, 0, NULL, &numDevices);
      *err = clGetDeviceIDs(platforms[0], device_type, 1, &(state->device), NULL);
   		assert(CL_SUCCESS == *err);
#else  
      *err = clGetDeviceIDs(NULL, device_type, 1, &(state->device), NULL);
#endif 
      //#####################################################################
		}

		assert(CL_SUCCESS == *err);

		/* Create a context to work with the device */
		state->context = clCreateContext(NULL, 1, &(state->device), NULL, NULL, err);
		assert(CL_SUCCESS == *err);

		/* Create a command queue for the context */
		state->queue = clCreateCommandQueue(state->context, state->device,
	  	                                    CL_QUEUE_PROFILING_ENABLE, NULL);
	}

	/*
	 * Device Information
	 */
	{
		cl_char vendor_name[1024] = {0};
		cl_char device_name[1024] = {0};

		/* Get information about the device */
		*err = clGetDeviceInfo(state->device, CL_DEVICE_VENDOR,
								sizeof(vendor_name), vendor_name, NULL);
		*err |= clGetDeviceInfo(state->device, CL_DEVICE_NAME,
								sizeof(device_name), device_name, NULL);
		assert(CL_SUCCESS == *err);

		cl_uint clock_freq_mhz = 0;
		*err = clGetDeviceInfo(state->device, CL_DEVICE_MAX_CLOCK_FREQUENCY,
								sizeof(clock_freq_mhz), &clock_freq_mhz, NULL);
		assert(CL_SUCCESS == *err);
	}
	return state;
}

/******************************************************************************
* Release Context and Command Queue
******************************************************************************/
void 
mcl_release_state (
	mcl_state_t * state
)
{
	clReleaseCommandQueue(state->queue);
	clReleaseContext(state->context);
}

/******************************************************************************
* Create Kernel
******************************************************************************/
cl_kernel 
mcl_create_kernel (
	mcl_state_t *state, 
	mcl_kernel_info_t *kernel_info,
	cl_int *err
)
{
	*err = CL_SUCCESS;
	
	const char *program_source = mcl_load_program_source("/media/ARM_DEV/work/dev/experimental/arndale/demos/product_74276M/benchmarks/suites/internal/image_filters/fir/fir_float/fir_kernels.cl");
	
	if (NULL == program_source)
	{
		printf("\nFailed to open file IG %s\n", kernel_info->file);
		*err = CL_INVALID_PROGRAM;
		return NULL;
	}
	
	cl_program program = clCreateProgramWithSource(state->context, 1, 
	                                               (const char **)&program_source,
	                                               NULL, err);
	assert(CL_SUCCESS == *err);
	
	*err = clBuildProgram(program, 0, NULL, kernel_info->options, NULL, NULL);
	if (*err != CL_SUCCESS)
	{
		char *buffer = NULL;
		size_t len;
		*err = clGetProgramBuildInfo(program, state->device, 
		                             CL_PROGRAM_BUILD_LOG, 0, buffer, &len);
		assert(CL_SUCCESS == *err);
		buffer = (char *) malloc(len);
		*err = clGetProgramBuildInfo(program, state->device, 
		                             CL_PROGRAM_BUILD_LOG, len, buffer, &len);
		assert(CL_SUCCESS == *err);
		printf("\nBuild log:\n%s\n", buffer);
		free(buffer);
	}
	
	cl_kernel kernel = clCreateKernel(program, kernel_info->name, err);
	assert(CL_SUCCESS == *err);

	mcl_release_program_source((char*)program_source);
	*err = clReleaseProgram( program );
	assert(CL_SUCCESS == *err);
	return kernel;
}

/******************************************************************************
* Create Kernel from binary
******************************************************************************/
cl_kernel 
mcl_create_kernel_binary (
	mcl_state_t *state, 
	mcl_kernel_info_t *kernel_info,
	cl_int *err
)
{
	*err = CL_SUCCESS;

	struct stat statbuf;
	
	const unsigned char *program_binary = mcl_load_program_binary(kernel_info->file, &statbuf);
	
	if (NULL == program_binary)
	{
		printf("\nFailed to open file %s\n", kernel_info->file);
		*err = CL_INVALID_PROGRAM;
		return NULL;
	}

	cl_program program = clCreateProgramWithBinary(state->context, 1, &state->device, (size_t *)&statbuf.st_size, (const unsigned char **)&program_binary, NULL, err);
	
	assert(CL_SUCCESS == *err);
	
	*err = clBuildProgram(program, 0, NULL, kernel_info->options, NULL, NULL);
	if (*err != CL_SUCCESS)
	{
		char *buffer = NULL;
		size_t len;
		*err = clGetProgramBuildInfo(program, state->device, 
		                             CL_PROGRAM_BUILD_LOG, 0, buffer, &len);
		assert(CL_SUCCESS == *err);
		buffer = (char *) malloc(len);
		*err = clGetProgramBuildInfo(program, state->device, 
		                             CL_PROGRAM_BUILD_LOG, len, buffer, &len);
		assert(CL_SUCCESS == *err);
		printf("\nBuild log:\n%s\n", buffer);
		free(buffer);
	}
	
	cl_kernel kernel = clCreateKernel(program, kernel_info->name, err);
	assert(CL_SUCCESS == *err);

	mcl_release_program_binary((char*)program_binary);
	*err = clReleaseProgram( program );
	assert(CL_SUCCESS == *err);

	return kernel;
}

/******************************************************************************
* Check Error Message
******************************************************************************/
bool 
mcl_check_err (
	cl_int err,
	const char* err_msg
)
{
	if (err == CL_SUCCESS)
	{
		return true;
	}

	/* some error has occurred */
	if (err_msg)
	{
		fprintf (stderr, "%s\n\terror_number: %d\n", err_msg, err);
	}
	else
	{
		fprintf (stderr, "error!");
	}
	
	/* decipher error code */
	switch (err)
	{
		case CL_SUCCESS:
			fprintf (stderr, "\tCL_SUCCESS\t\n");
			break;
		case CL_DEVICE_NOT_FOUND: 
			fprintf (stderr, "\tCL_DEVICE_NOT_FOUND\t\n");
			break;
		case CL_DEVICE_NOT_AVAILABLE:
			fprintf (stderr, "\tCL_DEVICE_NOT_AVAILABLE\t\n");
			break;
		case CL_COMPILER_NOT_AVAILABLE:
			fprintf (stderr, "\tCL_COMPILER_NOT_AVAILABLE\t\n");
			break;
		case CL_MEM_OBJECT_ALLOCATION_FAILURE:
			fprintf (stderr, "\tCL_MEM_OBJECT_ALLOCATION_FAILURE\t\n");
			break;
		case CL_OUT_OF_RESOURCES:
			fprintf (stderr, "\tCL_OUT_OF_RESOURCES\t\n");
			break;
		case CL_OUT_OF_HOST_MEMORY:
			fprintf (stderr, "\tCL_OUT_OF_HOST_MEMORY\t\n");
			break;
		case CL_PROFILING_INFO_NOT_AVAILABLE:
			fprintf (stderr, "\tCL_PROFILING_INFO_NOT_AVAILABLE\t\n");
			break;
		case CL_MEM_COPY_OVERLAP:
			fprintf (stderr, "\tCL_MEM_COPY_OVERLAP\t\n");
			break;
		case CL_IMAGE_FORMAT_MISMATCH:
			fprintf (stderr, "\tCL_IMAGE_FORMAT_MISMATCH\t\n");
			break;
		case CL_IMAGE_FORMAT_NOT_SUPPORTED:
			fprintf (stderr, "\tCL_IMAGE_FORMAT_NOT_SUPPORTED\t\n");
			break;
		case CL_BUILD_PROGRAM_FAILURE:
			fprintf (stderr, "\tCL_BUILD_PROGRAM_FAILURE\t\n");
			break;
		case CL_MAP_FAILURE:
			fprintf (stderr, "\tCL_MAP_FAILURE\t\n");
			break;
		case CL_INVALID_VALUE:
			fprintf (stderr, "\tCL_INVALID_VALUE\t\n");
			break;
		case CL_INVALID_DEVICE_TYPE:
			fprintf (stderr, "\tCL_INVALID_DEVICE_TYPE\t\n");
			break;
		case CL_INVALID_PLATFORM:
			fprintf (stderr, "\tCL_INVALID_PLATFORM\t\n");
			break;
		case CL_INVALID_DEVICE:
			fprintf (stderr, "\tCL_INVALID_DEVICE\t\n");
			break;
		case CL_INVALID_CONTEXT:
			fprintf (stderr, "\tCL_INVALID_CONTEXT\t\n");
			break;
		case CL_INVALID_QUEUE_PROPERTIES:
			fprintf (stderr, "\tCL_INVALID_QUEUE_PROPERTIES\t\n");
			break;
		case CL_INVALID_COMMAND_QUEUE:
			fprintf (stderr, "\tCL_INVALID_COMMAND_QUEUE\t\n");
			break;
		case CL_INVALID_HOST_PTR:
			fprintf (stderr, "\tCL_INVALID_HOST_PTR\t\n");
			break;
		case CL_INVALID_MEM_OBJECT:
			fprintf (stderr, "\tCL_INVALID_MEM_OBJECT\t\n");
			break;
		case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
			fprintf (stderr, "\tCL_INVALID_IMAGE_FORMAT_DESCRIPTOR\t\n");
			break;
		case CL_INVALID_IMAGE_SIZE:
			fprintf (stderr, "\tCL_INVALID_IMAGE_SIZE\t\n");
			break;
		case CL_INVALID_SAMPLER:
			fprintf (stderr, "\tCL_INVALID_SAMPLER\t\n");
			break;
		case CL_INVALID_BINARY:
			fprintf (stderr, "\tCL_INVALID_BINARY\t\n");
			break;
		case CL_INVALID_BUILD_OPTIONS:
			fprintf (stderr, "\tCL_INVALID_BUILD_OPTIONS\t\n");
			break;
		case CL_INVALID_PROGRAM:
			fprintf (stderr, "\tCL_INVALID_PROGRAM\t\n");
			break;
		case CL_INVALID_PROGRAM_EXECUTABLE:
			fprintf (stderr, "\tCL_INVALID_PROGRAM_EXECUTABLE\t\n");
			break;
		case CL_INVALID_KERNEL_NAME:
			fprintf (stderr, "\tCL_INVALID_KERNEL_NAME\t\n");
			break;
		case CL_INVALID_KERNEL_DEFINITION:
			fprintf (stderr, "\tCL_INVALID_KERNEL_DEFINITION\t\n");
			break;
		case CL_INVALID_KERNEL:
			fprintf (stderr, "\tCL_INVALID_KERNEL\t\n");
			break;
		case CL_INVALID_ARG_INDEX:
			fprintf (stderr, "\tCL_INVALID_ARG_INDEX\t\n");
			break;
		case CL_INVALID_ARG_VALUE:
			fprintf (stderr, "\tCL_INVALID_ARG_VALUE\t\n");
			break;
		case CL_INVALID_ARG_SIZE:
			fprintf (stderr, "\tCL_INVALID_ARG_SIZE\t\n");
			break;
		case CL_INVALID_KERNEL_ARGS:
			fprintf (stderr, "\tCL_INVALID_KERNEL_ARGS\t\n");
			break;
		case CL_INVALID_WORK_DIMENSION:
			fprintf (stderr, "\tCL_INVALID_WORK_DIMENSION\t\n");
			break;
		case CL_INVALID_WORK_GROUP_SIZE:
			fprintf (stderr, "\tCL_INVALID_WORK_GROUP_SIZE\t\n");
			break;
		case CL_INVALID_WORK_ITEM_SIZE:
			fprintf (stderr, "\tCL_INVALID_WORK_ITEM_SIZE\t\n");
			break;
		case CL_INVALID_GLOBAL_OFFSET:
			fprintf (stderr, "\tCL_INVALID_GLOBAL_OFFSET\t\n");
			break;
		case CL_INVALID_EVENT_WAIT_LIST:
			fprintf (stderr, "\tCL_INVALID_EVENT_WAIT_LIST\t\n");
			break;
		case CL_INVALID_EVENT:
			fprintf (stderr, "\tCL_INVALID_EVENT\t\n");
			break;
		case CL_INVALID_OPERATION: 
			fprintf (stderr, "\tCL_INVALID_OPERATION\t\n");
			break;
		case CL_INVALID_GL_OBJECT:
			fprintf (stderr, "\tCL_INVALID_GL_OBJECT\t\n");
			break;
		case CL_INVALID_BUFFER_SIZE:
			fprintf (stderr, "\tCL_INVALID_BUFFER_SIZE\t\n");
			break;
		case CL_INVALID_MIP_LEVEL: 
			fprintf (stderr, "\tCL_INVALID_MIP_LEVEL\t\n");
			break;
		default: fprintf (stderr, "\tunknown error...\n");
  }

  return false;
}
