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
 
#ifndef MCL_UTILS_H
#define MCL_UTILS_H

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define STR_LEN 1024 /**< Maximum string length */

/** \struct mcl_state_t
 *
 * @brief Properties of an OpenCL state.
 *
 * Structure describing the state.
 */
typedef struct{
	cl_device_id      device;        /**< Device associated with the state */
	cl_context        context;       /**< Context associated with the state. */
	cl_command_queue  queue;         /**< Command queue associated with the state. */
} mcl_state_t;

/** \struct mcl_kernel_info_t
 *
 * @brief Information about the OpenCL kernel.
 *
 * Structure describing the name, file and build option of the OpenCL kernel.
 */
typedef struct{
	const char *name;       /**< Kernel name */
	char file[STR_LEN];     /**< Absolute file path */
	char options[STR_LEN];  /**< Build options */
} mcl_kernel_info_t;

/**
 * @brief Load the program source from disk
 *
 * @return Pointer to string buffer containing program source.
 */
const char *
mcl_load_program_source (
	const char *filename
);

/**
 * @brief Release program source
 *
 * Frees memory allocated to the program source.
 */
void
mcl_release_program_source (
	char * source
);

/**
 * @brief Load the program binary from disk
 *
 * @return Pointer to string buffer containing program binary.
 */
const char *
mcl_load_program_binary (
	const char *filename
);

/**
 * @brief Release program binary
 *
 * Frees memory allocated to the program binary.
 */
void
mcl_release_program_binary (
	char * binary
);

/**
 * @brief Get context and command queue for the given device in a mcl_state structure.
 *
 * @return Pointer to \c mcl_execute_state structure (\e NULL on error).
 */
mcl_state_t *
mcl_create_state (
	cl_int *err
);

/**
 * @brief Get context and command queue for the given device in a mcl_state structure.
 *
 * @return Pointer to \c mcl_execute_state structure (\e NULL on error).
 */
mcl_state_t *
mcl_create_state (
	cl_int *err,
	const char *device_type_or_name
);

/**
 * @brief Releases an state instance.
 *
 * Frees memory allocated to the \c mcl_execute_state structure of the supplied 
 * state.
 *
 * @return Pointer to \c mcl_state_t structure (\e NULL on error).
 */
void
mcl_release_state (
	mcl_state_t * state
);


/**
 * @brief Creates a kernel object for a context with a GPU device.
 *
 * @return Kernel object on success, otherwise returns NULL.
 */
cl_kernel
mcl_create_kernel (
	mcl_state_t *state,
	mcl_kernel_info_t *kernel_info,
	cl_int *err
);

/**
 * @brief Creates a kernel object for a context with a GPU device.
 *
 * @return Kernel object on success, otherwise returns NULL.
 */
cl_kernel
mcl_create_kernel_binary (
	mcl_state_t *state,
	mcl_kernel_info_t *kernel_info,
	cl_int *err
);

/**
 * @brief check error message
 *
 * @return boolean type to see if err is success.
 */
bool mcl_check_err (
	cl_int err, 
	const char* err_msg
);

/** @def MCL_DECLARE_MEMORY
 *
 * @brief Declares and initializes a \c cl_mem variable to NULL.
 *
 * @param mem
 *            Memory object name.
 */
#define MCL_DECLARE_MEMORY(mem) cl_mem mem = NULL

/** @def MCL_DECLARE_PROGRAM
 *
 * @brief Declares and initializes a \c cl_program variable to NULL.
 *
 * @param program
 *                Program object name.
 */
#define MCL_DECLARE_PROGRAM(program) cl_program program = NULL

/** @def MCL_DECL_KERNEL
 *
 * @brief Declares and initializes a \c cl_kernel variable to NULL.
 *
 * @param kernel
 *               Kernel object name.
 */
#define MCL_DECLARE_KERNEL(kernel) cl_kernel kernel = NULL

/** @def MCL_DECLARE_STATE
 *
 * @brief Declares and initializes a pointer to \c mcl_execute_state structure 
 * variable to NULL.
 *
 * @param state
 *               Pointer variable name.
 */
#define MCL_DECLARE_STATE(state) mcl_state_t *state = NULL

/** @def MCL_CHECK_ERROR
 *
 * @brief Compares the return status of an OpenCL API call against the expected status.
 * If comparison fails write an error message to stderr and jump to \c exit label.
 *
 * @param status
 *               A return status of a OpenCL API call, it is of type \c cl_int
 * @param expected_status
 *               Value against which \c status is checked.
 * @param error_msg
 *               A text message which is to be printed when \c status != \c expected_status.
 */
#define MCL_CHECK_ERROR(status, expected_status, error_msg)                   \
do {                                                                          \
	if ((status) != (expected_status))                                        \
	{                                                                         \
		mcl_check_err(status, "\nError: " error_msg " \n");                   \
		goto exit;                                                            \
	}                                                                         \
} while (0)

/** @def MCL_CREATE_STATE
 *
 * @brief Calls the mcl_create_state function. Check for error against CL_SUCCESS.
 * If comparison fails write an error message to stderr and jump to \c exit label.
 *
 * @param state
 *            A pointer to \c mcl_execute_state structure
 */
#define MCL_CREATE_STATE(state)                                               \
do{                                                                           \
	(state) = mcl_create_state(&err);                                         \
	if(err != CL_SUCCESS)                                                     \
	{                                                                         \
		mcl_check_err(err, "\nError: mcl_create_state failed \n");            \
		goto exit;                                                            \
	}                                                                         \
}while(0)

/** @def MCL_CREATE_STATE_WITH_DEVICE
 *
 * @brief Calls the mcl_create_state function. Check for error against CL_SUCCESS.
 * If comparison fails write an error message to stderr and jump to \c exit label.
 *
 * @param state
 *            A pointer to \c mcl_execute_state structure
 */
#define MCL_CREATE_STATE_WITH_DEVICE(state, device_type_or_name)              \
do{                                                                           \
	(state) = mcl_create_state(&err, device_type_or_name);                    \
	if(err != CL_SUCCESS)                                                     \
	{                                                                         \
		mcl_check_err(err, "\nError: mcl_create_state failed \n");            \
		goto exit;                                                            \
	}                                                                         \
}while(0)

/** @def MCL_CREATE_KERNEL
 *
 * @brief Calls the mcl_create_kernel function. Check for error against CL_SUCCESS.
 * If comparison fails write an error message to stderr and jump to \c exit label.
 *
 * @param kernel
 *            Kernel object name.
 * @param state
 *            A pointer to \c mcl_execute_state structure
 * @param kernel_info
 *            Structure variable 
 */
#define MCL_CREATE_KERNEL(kernel, state, kernel_info)                         \
do{                                                                           \
	(kernel) = mcl_create_kernel(state, &kernel_info, &err);                  \
	if(err != CL_SUCCESS)                                                     \
	{                                                                         \
		mcl_check_err(err, "\nError: mcl_create_kernel failed \n");           \
		goto exit;                                                            \
	}                                                                         \
}while(0)

/** @def MCL_CREATE_KERNEL_BINARY
 *
 * @brief Calls the mcl_create_kernel_binary function. Check for error against CL_SUCCESS.
 * If comparison fails write an error message to stderr and jump to \c exit label.
 *
 * @param kernel
 *            Kernel object name.
 * @param state
 *            A pointer to \c mcl_execute_state structure
 * @param kernel_info
 *            Structure variable 
 */
#define MCL_CREATE_KERNEL_BINARY(kernel, state, kernel_info)                         \
do{                                                                           \
	(kernel) = mcl_create_kernel_binary(state, &kernel_info, &err);                  \
	if(err != CL_SUCCESS)                                                     \
	{                                                                         \
		mcl_check_err(err, "\nError: mcl_create_kernel_binary failed \n");           \
		goto exit;                                                            \
	}                                                                         \
}while(0)

/** @def MCL_RELEASE_MEMORY
 *
 * @brief Releases the memory object \c mem, if it is not NULL.
 *
 * @param mem
 *            Memory object name.
 */
#define MCL_RELEASE_MEMORY(mem)                                                      \
do {                                                                          \
	if(NULL != (mem))                                                         \
	{                                                                         \
		err = clReleaseMemObject((mem));                                      \
		if(err != CL_SUCCESS)                                                 \
		{                                                                     \
			mcl_check_err(err, "\nError: clReleaseMemObject failed \n");      \
		}                                                                     \
		(mem) = NULL;                                                         \
	}                                                                         \
} while (0)

/** @def MCL_RELEASE_PROGRAM
 *
 * @brief Releases the program object \c program, if it is not NULL.
 *
 * @param program
 *            Program object name.
 */
#define MCL_RELEASE_PROGRAM(program)                                                 \
do {                                                                          \
	if((program) != NULL)                                                     \
	{                                                                         \
		err = clReleaseProgram((program));                                    \
		if(err != CL_SUCCESS)                                                 \
		{                                                                     \
			mcl_check_err(err, "\nError: clReleaseProgram failed \n");        \
		}                                                                     \
		(program) = NULL;                                                     \
	}                                                                         \
} while (0)


/** @def MCL_RELEASE_KERNEL
 *
 * @brief Releases the kernel object \c kernel, if it is not NULL.
 *
 * @param kernel
 *            Kernel object name.
 */
#define MCL_RELEASE_KERNEL(kernel)                                                \
do {                                                                          \
	if((kernel) != NULL)                                                      \
	{                                                                         \
		err = clReleaseKernel((kernel));                                      \
		if(err != CL_SUCCESS)                                                 \
		{                                                                     \
			mcl_check_err(err, "\nError: clReleaseKernel failed \n");         \
		}                                                                     \
		(kernel) = NULL;                                                      \
	}                                                                         \
} while (0)

/** @def MCL_RELEASE_STATE
 *
 * @brief Release a pointer to \c mcl_execute_state structure 
 *
 * @param state
 *               Pointer variable name.
 */
#define MCL_RELEASE_STATE(state) mcl_release_state((state))

#endif /* MCL_UTILS_H */
