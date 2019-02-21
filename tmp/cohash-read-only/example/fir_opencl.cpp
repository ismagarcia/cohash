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

#include "fir_opencl.h"
#include "common/mcl_neon.h"
#include <vector>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <os/perf_event.h>
#include <os/pmf_perf_counter.h>
extern "C" {
#include <statsdb.h>
}

typedef enum hwc_ids
{
HWC_SHADER_CORE_0_ID,
HWC_SHADER_CORE_1_ID,
HWC_SHADER_CORE_2_ID,
HWC_SHADER_CORE_3_ID,
HWC_TILER_ID,
HWC_L2_AND_MMU_ID,
HWC_L3_AND_MMU_ID,
HWC_JOB_MANAGER_ID
}
hwc_ids;

typedef enum hwc_job_manager_ids
{
HWC_JOB_MANAGER_ID_TIMESTAMP_LO,
HWC_JOB_MANAGER_ID_TIMESTAMP_HI,
HWC_JOB_MANAGER_ID_PRFCNT_JM_EN,
HWC_JOB_MANAGER_ID_RESERVED_00,
HWC_JOB_MANAGER_ID_MESSAGES_SENT,
HWC_JOB_MANAGER_ID_MESSAGES_RECEIVED,
HWC_JOB_MANAGER_ID_GPU_ACTIVE,
HWC_JOB_MANAGER_ID_IRQ_ACTIVE,
HWC_JOB_MANAGER_ID_JS0_JOBS,
HWC_JOB_MANAGER_ID_JS0_TASKS,
HWC_JOB_MANAGER_ID_JS0_ACTIVE,
HWC_JOB_MANAGER_ID_RESERVED_01,
HWC_JOB_MANAGER_ID_JS0_WAIT_READ,
HWC_JOB_MANAGER_ID_JS0_WAIT_ISSUE,
HWC_JOB_MANAGER_ID_JS0_WAIT_DEPEND,
HWC_JOB_MANAGER_ID_JS0_WAIT_FINISH,
HWC_JOB_MANAGER_ID_JS1_JOBS,
HWC_JOB_MANAGER_ID_JS1_TASKS,
HWC_JOB_MANAGER_ID_JS1_ACTIVE,
HWC_JOB_MANAGER_ID_RESERVED_02,
HWC_JOB_MANAGER_ID_JS1_WAIT_READ,
HWC_JOB_MANAGER_ID_JS1_WAIT_ISSUE,
HWC_JOB_MANAGER_ID_JS1_WAIT_DEPEND,
HWC_JOB_MANAGER_ID_JS1_WAIT_FINISH,
HWC_JOB_MANAGER_ID_JS2_JOBS,
HWC_JOB_MANAGER_ID_JS2_TASKS,
HWC_JOB_MANAGER_ID_JS2_ACTIVE,
HWC_JOB_MANAGER_ID_RESERVED_03,
HWC_JOB_MANAGER_ID_JS2_WAIT_READ,
HWC_JOB_MANAGER_ID_JS2_WAIT_ISSUE,
HWC_JOB_MANAGER_ID_JS2_WAIT_DEPEND,
HWC_JOB_MANAGER_ID_JS2_WAIT_FINISH,
HWC_JOB_MANAGER_ID_JS3_JOBS,
HWC_JOB_MANAGER_ID_JS3_TASKS,
HWC_JOB_MANAGER_ID_JS3_ACTIVE,
HWC_JOB_MANAGER_ID_RESERVED_04,
HWC_JOB_MANAGER_ID_JS3_WAIT_READ,
HWC_JOB_MANAGER_ID_JS3_WAIT_ISSUE,
HWC_JOB_MANAGER_ID_JS3_WAIT_DEPEND,
HWC_JOB_MANAGER_ID_JS3_WAIT_FINISH,
HWC_JOB_MANAGER_ID_JS4_JOBS,
HWC_JOB_MANAGER_ID_JS4_TASKS,
HWC_JOB_MANAGER_ID_JS4_ACTIVE,
HWC_JOB_MANAGER_ID_RESERVED_05,
HWC_JOB_MANAGER_ID_JS4_WAIT_READ,
HWC_JOB_MANAGER_ID_JS4_WAIT_ISSUE,
HWC_JOB_MANAGER_ID_JS4_WAIT_DEPEND,
HWC_JOB_MANAGER_ID_JS4_WAIT_FINISH,
HWC_JOB_MANAGER_ID_JS5_JOBS,
HWC_JOB_MANAGER_ID_JS5_TASKS,
HWC_JOB_MANAGER_ID_JS5_ACTIVE,
HWC_JOB_MANAGER_ID_RESERVED_06,
HWC_JOB_MANAGER_ID_JS5_WAIT_READ,
HWC_JOB_MANAGER_ID_JS5_WAIT_ISSUE,
HWC_JOB_MANAGER_ID_JS5_WAIT_DEPEND,
HWC_JOB_MANAGER_ID_JS5_WAIT_FINISH,
HWC_JOB_MANAGER_ID_JS6_JOBS,
HWC_JOB_MANAGER_ID_JS6_TASKS,
HWC_JOB_MANAGER_ID_JS6_ACTIVE,
HWC_JOB_MANAGER_ID_RESERVED_07,
HWC_JOB_MANAGER_ID_JS6_WAIT_READ,
HWC_JOB_MANAGER_ID_JS6_WAIT_ISSUE,
HWC_JOB_MANAGER_ID_JS6_WAIT_DEPEND,
HWC_JOB_MANAGER_ID_JS6_WAIT_FINISH
}
hwc_job_manager_ids;

typedef enum hwc_tiler_ids
{
HWC_TILER_ID_TIMESTAMP_LO,
HWC_TILER_ID_TIMESTAMP_HI,
HWC_TILER_ID_PRFCNT_TILER_EN,
HWC_TILER_ID_JOBS_PROCESSED,
HWC_TILER_ID_TRIANGLES,
HWC_TILER_ID_QUADS,
HWC_TILER_ID_POLYGONS,
HWC_TILER_ID_POINTS,
HWC_TILER_ID_LINES,
HWC_TILER_ID_VCACHE_HIT,
HWC_TILER_ID_VCACHE_MISS,
HWC_TILER_ID_FRONT_FACING,
HWC_TILER_ID_BACK_FACING,
HWC_TILER_ID_PRIM_VISIBLE,
HWC_TILER_ID_PRIM_CULLED,
HWC_TILER_ID_PRIM_CLIPPED,
HWC_TILER_ID_LEVEL0,
HWC_TILER_ID_LEVEL1,
HWC_TILER_ID_LEVEL2,
HWC_TILER_ID_LEVEL3,
HWC_TILER_ID_LEVEL4,
HWC_TILER_ID_LEVEL5,
HWC_TILER_ID_LEVEL6,
HWC_TILER_ID_LEVEL7,
HWC_TILER_ID_COMMAND_1,
HWC_TILER_ID_COMMAND_2,
HWC_TILER_ID_COMMAND_3,
HWC_TILER_ID_COMMAND_4,
HWC_TILER_ID_COMMAND_4_7,
HWC_TILER_ID_COMMAND_8_15,
HWC_TILER_ID_COMMAND_16_63,
HWC_TILER_ID_COMMAND_64,
HWC_TILER_ID_COMPRESS_IN,
HWC_TILER_ID_COMPRESS_OUT,
HWC_TILER_ID_COMPRESS_FLUSH,
HWC_TILER_ID_TIMESTAMPS,
HWC_TILER_ID_PCACHE_HIT,
HWC_TILER_ID_PCACHE_MISS,
HWC_TILER_ID_PCACHE_LINE,
HWC_TILER_ID_PCACHE_STALL,
HWC_TILER_ID_WRBUF_HIT,
HWC_TILER_ID_WRBUF_MISS,
HWC_TILER_ID_WRBUF_LINE,
HWC_TILER_ID_WRBUF_PARTIAL,
HWC_TILER_ID_WRBUF_STALL,
HWC_TILER_ID_ACTIVE,
HWC_TILER_ID_LOADING_DESC,
HWC_TILER_ID_INDEX_WAIT,
HWC_TILER_ID_INDEX_RANGE_WAIT,
HWC_TILER_ID_VERTEX_WAIT,
HWC_TILER_ID_PCACHE_WAIT,
HWC_TILER_ID_WRBUF_WAIT,
HWC_TILER_ID_BUS_READ,
HWC_TILER_ID_BUS_WRITE,
HWC_TILER_ID_RESERVED_00,
HWC_TILER_ID_RESERVED_01,
HWC_TILER_ID_RESERVED_02,
HWC_TILER_ID_RESERVED_03,
HWC_TILER_ID_RESERVED_04,
HWC_TILER_ID_UTLB_STALL,
HWC_TILER_ID_UTLB_REPLAY_MISS,
HWC_TILER_ID_UTLB_REPLAY_FULL,
HWC_TILER_ID_UTLB_NEW_MISS,
HWC_TILER_ID_UTLB_HIT
}
hwc_tiler_ids;

typedef enum hwc_shader_core_ids
{
HWC_SHADER_CORE_TIMESTAMP_LO,
HWC_SHADER_CORE_TIMESTAMP_HI,
HWC_SHADER_CORE_PRFCNT_SHADER_EN,
HWC_SHADER_CORE_CYCLES_SHADER_CORE_ACTIVE,
HWC_SHADER_CORE_FRAGMENT_CYCLES_FRAGMENT_PROCESSING_ACTIVE,
HWC_SHADER_CORE_FRAGMENT_NUMBER_OF_PRIMITIVES,
HWC_SHADER_CORE_FRAGMENT_PRIMITIVES_DROPPED_DUE_TO_PLR_COVERAGE,
HWC_SHADER_CORE_FRAGMENT_CYCLES_SPENT_WAITING_FOR_DESCRIPTORS_BY_2,
#if ( ( MALI_CINSTR_HW == CINSTR_HW_T62X ) || ( MALI_CINSTR_HW == CINSTR_HW_T67X ) )
HWC_SHADER_CORE_FRAGMENT_CYCLES_ZS_RE_ORDER_BUFFER_CONTAINS_DATA,
#else
HWC_SHADER_CORE_FRAGMENT_CYCLES_SPENT_WAITING_FOR_PLR,
#endif
HWC_SHADER_CORE_FRAGMENT_CYCLES_SPENT_WAITING_FOR_VERTICES,
HWC_SHADER_CORE_FRAGMENT_CYCLES_SPENT_WAITING_FOR_TRISETUP,
#if ( ( MALI_CINSTR_HW == CINSTR_HW_T62X ) || ( MALI_CINSTR_HW == CINSTR_HW_T67X ) )
HWC_SHADER_CORE_FRAGMENT_CYCLES_PRE_TRIPIPE_BUFFER_CONTAINS_DATA,
#else
HWC_SHADER_CORE_FRAGMENT_CYCLES_SPENT_WAITING_FOR_RASTERIZER,
#endif
HWC_SHADER_CORE_FRAGMENT_NUMBER_OF_FRAGMENT_THREADS_STARTED,
HWC_SHADER_CORE_FRAGMENT_NUMBER_OF_DUMMY_THREADS_STARTED,
HWC_SHADER_CORE_FRAGMENT_QUADS_RASTERIZED,
HWC_SHADER_CORE_FRAGMENT_QUADS_DOING_EARLY_ZS_TEST,
HWC_SHADER_CORE_FRAGMENT_QUADS_KILLED_EARLY_ZS_TEST,
HWC_SHADER_CORE_FRAGMENT_THREADS_DOING_LATE_ZS_TEST,
HWC_SHADER_CORE_FRAGMENT_THREADS_KILLED_LATE_ZS_TEST,
HWC_SHADER_CORE_FRAGMENT_CYCLES_NO_PHYSICAL_TILE_BUFFERS_AVAILABLE_FOR_RENDERING,
HWC_SHADER_CORE_FRAGMENT_NUMBER_OF_TILES_RENDERED,
HWC_SHADER_CORE_FRAGMENT_TRANSACTION_ELIMINATION_SIGNATURE_MATCHES,
HWC_SHADER_CORE_COMPUTE_CYCLES_COMPUTE_PROCESSING_ACTIVE,
HWC_SHADER_CORE_COMPUTE_NUMBER_OF_TASKS,
HWC_SHADER_CORE_COMPUTE_NUMBER_OF_COMPUTE_THREADS_STARTED,
HWC_SHADER_CORE_COMPUTE_CYCLES_SPENT_WAITING_FOR_DESCRIPTORS,
HWC_SHADER_CORE_TRIPIPE_CYCLES_TRIPIPE_ACTIVE,
HWC_SHADER_CORE_ARITH_NUMBER_OF_INSTRUCTION_WORDS_BY_NUMBER_OF_PIPES,
HWC_SHADER_CORE_ARITH_CYCLES_LOST_DUE_TO_REGISTER_SCHEDULING_BY_NUMBER_OF_PIPES,
HWC_SHADER_CORE_ARITH_CYCLES_LOST_DUE_TO_L0_INSTRUCTION_CACHE_MISS_BY_NUMBER_OF_PIPES,
HWC_SHADER_CORE_ARITH_FRAGMENT_DEPENDENCY_CHECK_FAILURES_BY_NUMBER_OF_PIPES,
HWC_SHADER_CORE_LS_NUMBER_OF_INSTRUCTION_WORDS_COMPLETED,
HWC_SHADER_CORE_LS_NUMBER_OF_FULL_PIPELINE_ISSUES,
HWC_SHADER_CORE_LS_RESTARTS_DUE_TO_UNPAIRABLE_INSTRUCTIONS,
HWC_SHADER_CORE_LS_FULL_PIPE_RE_ISSUE_CACHE_MISS_OR_TLB,
HWC_SHADER_CORE_LS_FULL_PIPE_RE_ISSUE_VARYING_DATA_UNAVAILABLE,
HWC_SHADER_CORE_LS_FULL_PIPE_RE_ISSUE_ATTRIBUTE_CACHE_MISS,
HWC_SHADER_CORE_LS_WRITEBACK_NOT_USED,_AND_SOMETHING_INTO_PAUSE_BUFFER,
HWC_SHADER_CORE_TEX_INSTRUCTION_WORDS_COMPLETED,
HWC_SHADER_CORE_TEX_BUBBLES_WITH_THREADS_WAITING_FOR_BARRIER,
HWC_SHADER_CORE_TEX_INSTRUCTION_WORDS_RESTART_LOOP_1_DUE_TO_L0_INSTRUCTION_CACHE_MISS,
HWC_SHADER_CORE_TEX_INSTRUCTION_WORDS_RESTART_LOOP_1_DUE_TO_DESCRIPTOR_MISSES,
HWC_SHADER_CORE_TEX_NUMBER_OF_THREADS_THROUGH_LOOP_2_ADDRESS_CALCULATION,
HWC_SHADER_CORE_TEX_RECIRCULATE_DUE_TO_COMPLETE_TEXTURE_CACHE_MISS,
HWC_SHADER_CORE_TEX_RECIRCULATE_DUE_TO_SURFACE_DESCRIPTOR_MISS,
HWC_SHADER_CORE_TEX_RECIRCULATE_DUE_TO_MULTIPASS,
HWC_SHADER_CORE_TEX_RECIRCULATE_DUE_TO_PARTIAL_CACHE_MISS,
HWC_SHADER_CORE_TEX_RECIRCULATE_DUE_TO_TEXTURE_CACHE_BANK_CONFLICT,
HWC_SHADER_CORE_LSC_READ_HITS,
HWC_SHADER_CORE_LSC_READ_MISSES,
HWC_SHADER_CORE_LSC_WRITE_HITS,
HWC_SHADER_CORE_LSC_WRITE_MISSES,
HWC_SHADER_CORE_LSC_ATOMIC_HITS,
HWC_SHADER_CORE_LSC_ATOMIC_MISSES,
HWC_SHADER_CORE_LSC_LINE_FETCHES,
HWC_SHADER_CORE_LSC_DIRTY_LINE_EVICTIONS,
HWC_SHADER_CORE_LSC_SNOOPS,
HWC_SHADER_CORE_AXI_UTLB_AXI_ADDRESS_CHANNEL_STALL_GENERATED,
HWC_SHADER_CORE_AXI_UTLB_NEW_REQUEST_CACHE_MISS,
HWC_SHADER_CORE_AXI_UTLB_NUMBER_OF_AXI_TRANSACTIONS,
HWC_SHADER_CORE_LS_UTLB_CACHE_MISS_BY_NUMBER_OF_INTERFACES,
HWC_SHADER_CORE_LS_UTLB_CACHE_HIT_BY_NUMBER_OF_INTERFACES,
HWC_SHADER_CORE_AXI_BEATS_READ,
HWC_SHADER_CORE_AXI_BEATS_WRITTEN
}
hwc_shader_core_ids;

typedef enum hwc_l2_and_mmu_ids
{
HWC_L2_AND_MMU_ID_TIMESTAMP_LO,
HWC_L2_AND_MMU_ID_TIMESTAMP_HI,
HWC_L2_AND_MMU_ID_PRFCNT_MMU_L2_EN,
HWC_L2_AND_MMU_ID_RESERVED,
HWC_L2_AND_MMU_ID_MMU_TABLE_WALK,
HWC_L2_AND_MMU_ID_MMU_REPLAY_MISS,
HWC_L2_AND_MMU_ID_MMU_REPLAY_FULL,
HWC_L2_AND_MMU_ID_MMU_NEW_MISS,
HWC_L2_AND_MMU_ID_MMU_HIT,
HWC_L2_AND_MMU_ID_RESERVED_00,
HWC_L2_AND_MMU_ID_RESERVED_01,
HWC_L2_AND_MMU_ID_RESERVED_02,
HWC_L2_AND_MMU_ID_RESERVED_03,
HWC_L2_AND_MMU_ID_RESERVED_04,
HWC_L2_AND_MMU_ID_RESERVED_05,
HWC_L2_AND_MMU_ID_RESERVED_06,
HWC_L2_AND_MMU_ID_UTLB_STALL,
HWC_L2_AND_MMU_ID_UTLB_REPLAY_MISS,
HWC_L2_AND_MMU_ID_UTLB_REPLAY_FULL,
HWC_L2_AND_MMU_ID_UTLB_NEW_MISS,
HWC_L2_AND_MMU_ID_UTLB_HIT,
HWC_L2_AND_MMU_ID_RESERVED_07,
HWC_L2_AND_MMU_ID_RESERVED_08,
HWC_L2_AND_MMU_ID_RESERVED_09,
HWC_L2_AND_MMU_ID_RESERVED_10,
HWC_L2_AND_MMU_ID_RESERVED_11,
HWC_L2_AND_MMU_ID_RESERVED_12,
HWC_L2_AND_MMU_ID_RESERVED_13,
HWC_L2_AND_MMU_ID_RESERVED_14,
HWC_L2_AND_MMU_ID_RESERVED_15,
HWC_L2_AND_MMU_ID_L2_EXT_WRITE_BEATS,
HWC_L2_AND_MMU_ID_L2_EXT_READ_BEATS,
HWC_L2_AND_MMU_ID_L2_ANY_LOOKUP,
HWC_L2_AND_MMU_ID_L2_READ_LOOKUP,
HWC_L2_AND_MMU_ID_L2_SREAD_LOOKUP,
HWC_L2_AND_MMU_ID_L2_READ_REPLAY,
HWC_L2_AND_MMU_ID_L2_READ_SNOOP,
HWC_L2_AND_MMU_ID_L2_READ_HIT,
HWC_L2_AND_MMU_ID_L2_CLEAN_MISS,
HWC_L2_AND_MMU_ID_L2_WRITE_LOOKUP,
HWC_L2_AND_MMU_ID_L2_SWRITE_LOOKUP,
HWC_L2_AND_MMU_ID_L2_WRITE_REPLAY,
HWC_L2_AND_MMU_ID_L2_WRITE_SNOOP,
HWC_L2_AND_MMU_ID_L2_WRITE_HIT,
HWC_L2_AND_MMU_ID_L2_EXT_READ_FULL,
HWC_L2_AND_MMU_ID_L2_EXT_READ_HALF,
HWC_L2_AND_MMU_ID_L2_EXT_WRITE_FULL,
HWC_L2_AND_MMU_ID_L2_EXT_WRITE_HALF,
HWC_L2_AND_MMU_ID_L2_EXT_READ,
HWC_L2_AND_MMU_ID_L2_EXT_READ_LINE,
HWC_L2_AND_MMU_ID_L2_EXT_WRITE,
HWC_L2_AND_MMU_ID_L2_EXT_WRITE_LINE,
HWC_L2_AND_MMU_ID_L2_EXT_WRITE_SMALL,
HWC_L2_AND_MMU_ID_L2_EXT_BARRIER,
HWC_L2_AND_MMU_ID_L2_EXT_AR_STALL,
HWC_L2_AND_MMU_ID_L2_EXT_R_BUF_FULL,
HWC_L2_AND_MMU_ID_L2_EXT_RD_BUF_FULL,
HWC_L2_AND_MMU_ID_L2_EXT_R_RAW,
HWC_L2_AND_MMU_ID_L2_EXT_W_STALL,
HWC_L2_AND_MMU_ID_L2_EXT_W_BUF_FULL,
HWC_L2_AND_MMU_ID_L2_EXT_R_W_HAZARD,
HWC_L2_AND_MMU_ID_L2_TAG_HAZARD,
HWC_L2_AND_MMU_ID_L2_SNOOP_FULL,
HWC_L2_AND_MMU_ID_L2_REPLAY_FULL
}
hwc_l2_and_mmu_ids;

/* This function is defined in libmali.so when unit=1 (see MIDCL-3798) */
//#############################################
// IG 03/12/2012 ismael.garcia@arm.com
// Note: libmali requires to be compiled with
// MALI_UNIT_TEST=1, and this seems to be enabled
// by using unit=1 in scons command...
//#############################################
extern "C" void mcl_plugin_device_arm_set_num_of_cpu_cores( int n ) __attribute__((weak));
extern "C" void mcl_plugin_device_arm_explicitly_start_threads_migrated( int n ) __attribute__((weak));
extern "C" void mcl_plugin_device_t6xx_device_hwc(cl_context context, int hwc_type, int hwc_id, unsigned int* hwc, int hwc_debug) __attribute__((weak));

#define NUM_CPUS 3
struct stats_cpufreq stats[2][NUM_CPUS];

/**
 * @brief Set OpenCL kernel info structure:
 *        absolute file path, 
 *        kernel name
 */
static void 
set_kernel_info (
	const fir_params& params,         //< Parameters.
	mcl_kernel_info_t* kernel_info,   //< Kernel info.
	cl_int* err
)
{
	*err = CL_SUCCESS;
	kernel_info->name = params.kernel_name;
	snprintf(kernel_info->file, STR_LEN, "%s",
	         params.kernel_path);
	snprintf(kernel_info->options, STR_LEN, "");
}

/**
 * @brief Print benchmark output.
 */
static void print_stats(
	const fir_params & params, //< Parameters.
	const timing_t & timing              //< Total execution time (ns).
)
{
}

/* OpenCL benchmark application code */
unsigned long long
fir_run_cl (
	const fir_params& params,
	const float* const input,
	float* const output,
	const unsigned int n_rows,
	const unsigned int n_cols,
	size_t * lws,
  struct statvar& ocl_single_time, 
  struct statvar& ocl_single_cpu0_load, 
  struct statvar& ocl_single_cpu1_load, 
  struct statvar& ocl_single_gpu0_load, 
  struct statvar& ocl_single_power_c0, 
  struct statvar& ocl_single_power_c1,
  struct statvar& ocl_all_time, 
  struct statvar& ocl_all_cpu0_load, 
  struct statvar& ocl_all_cpu1_load, 
  struct statvar& ocl_all_gpu0_load, 
  struct statvar& ocl_all_power_c0, 
  struct statvar& ocl_all_power_c1,
	bool single_core = false
)
{

  //#################################################
  // IG 05/02/2013 ismael.garcia@arm.com
  //timeval tv0, tv1;
  //pid = getpid();
  //counter = new pmf_perf_counter(-1, pid, pmf_perf_counter::PAGE_FAULTS_COUNT);
  //counter->start();
  //getPowerCounters(valueC0Start, valueC1Start);
  //gettimeofday( &tv0, NULL );
  //#################################################

  double valueC0Start, valueC0End, valueC1Start, valueC1End, valueC0Diff, valueC1Diff;

	cl_int err = CL_SUCCESS;
#ifndef FREEOCL        
  if (params.cl_device_type_or_name == "cpu")
  {
	  if (single_core)
	  {
		  /* The mcl_plugin_device_arm_set_num_of_cpu_cores function is not always available */
		  if (NULL == mcl_plugin_device_arm_set_num_of_cpu_cores)
		  {
			  std::cerr << "Error: Driver does not support limiting number of cores" << std::endl;
		  }
		  else
		  {
			  /* Limit number of cores used to concurrently process workgroups for a given job to 1 */
			  mcl_plugin_device_arm_set_num_of_cpu_cores(1);
		  }
		  if (NULL != mcl_plugin_device_arm_explicitly_start_threads_migrated)
		  {
			  /* Reset thread migration */
			  mcl_plugin_device_arm_explicitly_start_threads_migrated(0);
		  }
	  }
	  else
	  {
		  if (NULL != mcl_plugin_device_arm_set_num_of_cpu_cores)
		  {
			  /* Reset core limit */
			  mcl_plugin_device_arm_set_num_of_cpu_cores(0);
		  }
		  /* The mcl_plugin_device_arm_explicitly_start_threads_migrated function is not always available */
		  if (NULL == mcl_plugin_device_arm_explicitly_start_threads_migrated)
		  {
			  std::cerr << "Error: Driver does not support starting threads explicitly migrated" << std::endl;
		  }
		  else
		  {
			  /* Limit number of cores used to concurrently process workgroups for a given job to 1 */
			  mcl_plugin_device_arm_explicitly_start_threads_migrated(1);
		  }
	  }
  }
#endif       
	
#ifdef __ARM_NEON__ 
	bool counters_enabled = false;
	long long unsigned int cycles = 0;
#endif
	timeval tim;
	double t1, t2;
  pid_t pid;
  pmf_perf_counter *counter;

	mcl_profiler p;
	timing_t timing;
	cl_double pixels_per_cycle;
	
	mcl_kernel_info_t kernel_info;
	set_kernel_info(params, &kernel_info, &err);

	const size_t output_size = n_rows * n_cols * sizeof(float);

	int n_padded_rows = n_rows + params.n_padding_rows;
	int n_padded_cols = n_cols + params.n_padding_cols;

	const size_t input_size = n_padded_rows * n_padded_cols * sizeof(float);
	
	std::vector<float> weights(params.n_size * params.n_size);
	const cl_int n_size = params.n_size;
	for(std::vector<float>::iterator it = weights.begin(); it != weights.end(); ++it)
	{
		*it = 1.0f / (float)(params.n_size * params.n_size);
	}

	const size_t weights_size = weights.size() * sizeof(float);

	MCL_DECLARE_KERNEL(kernel);
	MCL_DECLARE_MEMORY(input_mem);
	MCL_DECLARE_MEMORY(output_mem);
	MCL_DECLARE_MEMORY(weight_mem);
	MCL_DECLARE_STATE(state);
	
	MCL_CREATE_STATE_WITH_DEVICE(state, params.cl_device_type_or_name.c_str());

  MCL_CREATE_KERNEL(kernel, state, kernel_info);

    //#################################################
    // IG 27/02/2013 ismael.garcia@arm.com
    for (int i = 0; i < params.runs; i++)
    {   
      stats_cpufreq( getpid(), stats[0], NUM_CPUS );  
      timeval tv0, tv1;
      pid = getpid();
      counter = new pmf_perf_counter(-1, pid, pmf_perf_counter::PAGE_FAULTS_COUNT);
      counter->start();
      getPowerCounters(valueC0Start, valueC1Start);
      gettimeofday( &tv0, NULL );
    //#################################################	


  /* Allocate device memory for the input data */
	input_mem = clCreateBuffer(state->context, CL_MEM_READ_ONLY, 
	                           input_size, NULL, &err);
	MCL_CHECK_ERROR(err, CL_SUCCESS, "clCreateBuffer: creating input_mem failed");
	
	/* Copy the input data to the device */
	err = clEnqueueWriteBuffer(state->queue, input_mem, CL_TRUE, 0, input_size,
	                           (void*)input, 0, NULL, NULL);
	MCL_CHECK_ERROR(err, CL_SUCCESS, "clEnqueueWriteBuffer: writing to input_mem failed");


	if (strcmp(params.kernel_name, "fir7x7_f_tex") == 0)
	{
		float cl_weights[56];
		for (int y = 0; y < 7; y++)
			for (int x = 0; x < 7; x++)
				cl_weights[y*8 + x] = weights[y*7 + x];

		cl_image_format format;
		format.image_channel_order = CL_RGBA;
		format.image_channel_data_type = CL_FLOAT;

		const size_t origin[3] = {0, 0, 0};
		const size_t region[3] = {2, 7, 1};

		weight_mem = clCreateImage2D(state->context, CL_MEM_READ_ONLY, &format,
				2, 7, 0, NULL, &err);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clCreateImage2D: creating weights_mem failed");

		err = clEnqueueWriteImage(state->queue, weight_mem, CL_TRUE, origin, region,
				0, 0, (void*)cl_weights, 0, NULL, NULL);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clEnqueueWriteImage: writing to weight_mem failed");
	}
	else if (strcmp(params.kernel_name, "fir7x7_f") == 0)
	{
		/* Allocate device memory for the fir weights */
		weight_mem = clCreateBuffer(state->context, CL_MEM_READ_ONLY,
				weights_size + 4, NULL, &err);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clCreateBuffer: creating weight_mem failed");

		/* Copy the weights to the device */
		err = clEnqueueWriteBuffer(state->queue, weight_mem, CL_TRUE, 0, weights_size,
				(void*)&weights[0], 0, NULL, NULL);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clEnqueueWriteBuffer: writing to weight_mem failed");
	}
	else
	{
		/* Allocate device memory for the fir weights */
		weight_mem = clCreateBuffer(state->context, CL_MEM_READ_ONLY,
				weights_size, NULL, &err);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clCreateBuffer: creating weight_mem failed");

		/* Copy the weights to the device */
		err = clEnqueueWriteBuffer(state->queue, weight_mem, CL_TRUE, 0, weights_size,
				(void*)&weights[0], 0, NULL, NULL);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clEnqueueWriteBuffer: writing to weight_mem failed");
	}
	
	/* Allocate device memory for the output data */
	output_mem = clCreateBuffer(state->context, CL_MEM_READ_WRITE, 
	                            output_size, NULL, &err);
	MCL_CHECK_ERROR(err, CL_SUCCESS, "clCreateBuffer: creating output_mem failed");

        /* Wait for completion */
	err = clFinish(state->queue);
	MCL_CHECK_ERROR(err, CL_SUCCESS, "clFinish: memory operations failed");

        /* Setup the kernel arguments */
	if(strcmp(params.kernel_name, "firnxn_f") == 0)
	{
		err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_mem);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting input_mem failed");

		err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_mem);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting output_mem failed");

		err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &weight_mem);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting weight_mem failed");

		err = clSetKernelArg(kernel, 3, sizeof(cl_int), &n_padded_cols);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting n_padded_cols failed");

		err = clSetKernelArg(kernel, 4, sizeof(cl_int), &n_cols);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting n_cols failed");

		err = clSetKernelArg(kernel, 5, sizeof(cl_int), &n_size);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting n_size failed");
	}
	else if(strcmp(params.kernel_name, "fir7x7_f") == 0 ||
		    strcmp(params.kernel_name, "fir7x7_f_tex") == 0)
	{
		err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_mem);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting input_mem failed");

		err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_mem);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting output_mem failed");

		err = clSetKernelArg(kernel, 2, sizeof(cl_int), &n_padded_cols);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting n_padded_cols failed");

		err = clSetKernelArg(kernel, 3, sizeof(cl_int), &n_cols);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting n_cols failed");

		err = clSetKernelArg(kernel, 4, sizeof(cl_mem), &weight_mem);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting weight_mem failed");
	}
	else if (strcmp(params.kernel_name, "fir3x3_f_neon") == 0)
	{
		err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_mem);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting input_mem failed");

		err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_mem);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting output_mem failed");
		
		err = clSetKernelArg(kernel, 2, sizeof(cl_int), &n_padded_cols);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting n_padded_cols failed");
		
		err = clSetKernelArg(kernel, 3, sizeof(cl_int), &n_cols);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting n_cols failed");
		err = clSetKernelArg(kernel, 4, sizeof(cl_uint), &n_rows);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting n_rows failed");
		
		params.lws[0] = 1;
		params.gws[0] = 1;
		params.lws[1] = 1;
		params.gws[1] = 1;

	}
	else
	{
		err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_mem);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting input_mem failed");

		err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_mem);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting output_mem failed");

		err = clSetKernelArg(kernel, 2, sizeof(cl_int), &n_padded_cols);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting n_padded_cols failed");

		err = clSetKernelArg(kernel, 3, sizeof(cl_int), &n_cols);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "clSetKernelArg: setting n_cols failed");
	}

  if ( NULL == lws )
	{
		lws = params.lws;
	}

	/* Make sure that we don't exceed the maximum local work group size */
	{
		size_t max_work_group_size;
		size_t max_work_item_sizes[3];
		size_t max_lws[2];

		err = clGetDeviceInfo(state->device, CL_DEVICE_MAX_WORK_GROUP_SIZE,
			sizeof(max_work_group_size), &max_work_group_size, NULL);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "Failed to get max work group size");

		err = clGetDeviceInfo(state->device, CL_DEVICE_MAX_WORK_ITEM_SIZES,
			sizeof(max_work_item_sizes), max_work_item_sizes, NULL);
		MCL_CHECK_ERROR(err, CL_SUCCESS, "Failed to get max work group sizes");

		max_lws[0] = max_lws[1] = (size_t)sqrt((double)max_work_group_size);
		max_lws[0] = max_work_item_sizes[0] < max_lws[0] ? max_work_item_sizes[0] : max_lws[0];
		max_lws[1] = max_work_item_sizes[1] < max_lws[1] ? max_work_item_sizes[1] : max_lws[1];

		while (params.gws[0] % lws[0] != 0 || lws[0] > max_lws[0]) lws[0]--;
		while (params.gws[1] % lws[1] != 0 || lws[1] > max_lws[1]) lws[1]--;
	}

	//#################################################
	// IG 04/12/2012 ismael.garcia@arm.com
	//printf("Using local work size {%d, %d} and global work size {%d, %d} (each work-item processes 4 pixels)\n", lws[0], lws[1], params.gws[0], params.gws[1]);
	//#################################################

	/* Enqueue the kernel */
    //#################################################
    // IG 27/02/2013 ismael.garcia@arm.com
    //for (int i = 0; i < params.runs; i++)
    //{   
    //  stats_cpufreq( getpid(), stats[0], NUM_CPUS );  
    //  timeval tv0, tv1;
    //  pid = getpid();
    //  counter = new pmf_perf_counter(-1, pid, pmf_perf_counter::PAGE_FAULTS_COUNT);
    //  counter->start();
    //  getPowerCounters(valueC0Start, valueC1Start);
    //  gettimeofday( &tv0, NULL );
    //#################################################

	  err = clEnqueueNDRangeKernel(state->queue, kernel, params.work_dim, NULL, 
	                               params.gws, lws, 0, NULL, &(p.event));
	  MCL_CHECK_ERROR(err, CL_SUCCESS, "clEnqueueNDRangeKernel: enqueuing kernel failed");
#if ENABLE_CHECK_FIR_FLOAT_ACTIVE_THREADS
	  system("ps -AL -o user,pid,pcpu,cpuid,cmd > /tmp/fir_float_ocl_neon_threads.txt && gedit /tmp/fir_float_ocl_neon_threads.txt");
#endif
	  /* Wait for completion */
	  err = clFinish(state->queue);
	  MCL_CHECK_ERROR(err, CL_SUCCESS, "clFinish: terminating kernel failed");

	  /* Collect timing info */
	  err = mcl_fill_timing_info(&p);
	  MCL_CHECK_ERROR(err, CL_SUCCESS, "mcl_fill_timing_info: failed");
	
	  clReleaseEvent( p.event );
	  timing.total_time_ns = mcl_start_to_end_time(&p);

    //#################################################
    // IG 30/01/2013 ismael.garcia@arm.com 
    gettimeofday( &tv1, NULL );
    getPowerCounters(valueC0End, valueC1End);
    timing.total_time_ns = (tv1.tv_sec * 1000000000ull + tv1.tv_usec * 1000ull) - (tv0.tv_sec * 1000000000ull + tv0.tv_usec * 1000ull);
    counter->stop();
	  printf("ocl_%s PAGE_FAULTS_COUNT: %lld\n", (single_core) ? "single" : "all",  counter->value());
	  delete counter;
    //#################################################

    stats_cpufreq( getpid(), stats[1], NUM_CPUS );

    char output_full_name[1024];
    sprintf(output_full_name, "./l2_mmu_%dx%d_%d_null.dat", params.width, params.height, i);

    int bak, newf;
    fflush(stdout);
    bak = dup(1);
    newf = open(output_full_name, O_WRONLY);
    dup2(newf, 1);
    close(newf);

    sprintf(output_full_name, "./l2_mmu_%dx%d_%d.dat", params.width, params.height, i);
    FILE *output = fopen(output_full_name, "wb");
    if (dup2(fileno(output),1) == -1)
    {
        exit(EXIT_FAILURE);
    }

    //if (i == 2)
    //  mcl_plugin_device_t6xx_device_hwc(state->context, HWC_L2_AND_MMU_ID, HWC_L2_AND_MMU_ID_L2_READ_HIT, &stats[1][0].gpu_l2_read_hit, 1);
    //if (i == 3)
    //  mcl_plugin_device_t6xx_device_hwc(state->context, HWC_TILER_ID, HWC_TILER_ID_VCACHE_HIT, &stats[1][0].gpu_l2_read_hit, 1);
    //if (i == 4)
    //  mcl_plugin_device_t6xx_device_hwc(state->context,HWC_SHADER_CORE_0_ID, HWC_SHADER_CORE_LSC_READ_HITS, &stats[1][0].gpu_shader_core0_lsc_read_hit, 1);
    //if (i == 5)
    //  mcl_plugin_device_t6xx_device_hwc(state->context,HWC_SHADER_CORE_1_ID, HWC_SHADER_CORE_LSC_READ_HITS, &stats[1][0].gpu_shader_core1_lsc_read_hit, 1);
    //if (i == 6)
    //  mcl_plugin_device_t6xx_device_hwc(state->context,HWC_SHADER_CORE_2_ID, HWC_SHADER_CORE_LSC_READ_HITS, &stats[1][0].gpu_shader_core2_lsc_read_hit, 1);
    //if (i == 7)
    //  mcl_plugin_device_t6xx_device_hwc(state->context,HWC_SHADER_CORE_3_ID, HWC_SHADER_CORE_LSC_READ_HITS, &stats[1][0].gpu_shader_core3_lsc_read_hit, 1);
    //if (i == 9)
    //  mcl_plugin_device_t6xx_device_hwc(state->context, HWC_JOB_MANAGER_ID, HWC_JOB_MANAGER_ID_GPU_ACTIVE, &stats[1][0].gpu_active, 1);

    fflush(stdout);
    dup2(fileno(output), 1);
    fclose(output);

    fflush(stdout);
    dup2(bak, 1);
    close(bak);

    float cpu_load0 = stats[1][1].scale * 100.0f * (stats[1][1].load - stats[0][1].load) / (stats[1][1].total - stats[0][1].total);
    float cpu_load1 = stats[1][2].scale * 100.0f * (stats[1][2].load - stats[0][2].load) / (stats[1][2].total - stats[0][2].total);
    float gpu_load = ((float)(stats[1][0].gpu_active)) * 100000.0f / (533.0f * ((float)timing.total_time_ns));
  
#if THERMAL_VALUES
    valueC0Diff = valueC0End;
    valueC1Diff = valueC1End;
#else
    valueC0Diff = valueC0End - valueC0Start;
    valueC1Diff = valueC1End - valueC1Start;
#endif

    if (single_core)
    {
      updatePowerCounters(&ocl_single_power_c0, &ocl_single_power_c1, valueC0Diff, valueC1Diff);
      statvar_set(&ocl_single_time, timing.total_time_ns);
      if (cpu_load0 != NAN) statvar_set(&ocl_single_cpu0_load, cpu_load0);
      if (cpu_load1 != NAN) statvar_set(&ocl_single_cpu1_load, cpu_load1);
      if (gpu_load  != NAN) statvar_set(&ocl_single_gpu0_load, gpu_load);
    }
    else
    {
      updatePowerCounters(&ocl_all_power_c0, &ocl_all_power_c1, valueC0Diff, valueC1Diff);
      statvar_set(&ocl_all_time, timing.total_time_ns);
      if (cpu_load0 != NAN) statvar_set(&ocl_all_cpu0_load, cpu_load0);
      if (cpu_load1 != NAN) statvar_set(&ocl_all_cpu1_load, cpu_load1);
      if (gpu_load  != NAN) statvar_set(&ocl_all_gpu0_load, gpu_load);
    }
  }

	err = clGetDeviceInfo(state->device, CL_DEVICE_MAX_CLOCK_FREQUENCY,
	                      sizeof(cl_uint), &(timing.freq_mhz), NULL);
	MCL_CHECK_ERROR(err, CL_SUCCESS, "clGetDeviceInfo: failed");
	
	print_stats(params, timing);
	
	/* Copy results from device to host */
	err = clEnqueueReadBuffer(state->queue, output_mem, CL_TRUE, 0, output_size,
	                          output, 0, NULL, NULL);
	MCL_CHECK_ERROR(err, CL_SUCCESS, "clEnqueueReadBuffer: reading from output_mem failed");

	/* Wait for completion */
	err = clFinish(state->queue);
	MCL_CHECK_ERROR(err, CL_SUCCESS, "clFinish: memory operations failed");
	
exit:
	/* Release OpenCL kernel */
	MCL_RELEASE_KERNEL(kernel);
	
	/* Release all OpenCL memory objects */
	MCL_RELEASE_MEMORY(input_mem);
//	MCL_RELEASE_MEMORY(texture_mem);
	MCL_RELEASE_MEMORY(output_mem);
	MCL_RELEASE_MEMORY(weight_mem);
	
	/* Release the state */
	MCL_RELEASE_STATE(state);
	
  //#################################################
  // IG 30/01/2013 ismael.garcia@arm.com 
  //gettimeofday( &tv1, NULL );
  //getPowerCounters(valueC0End, valueC1End);
  //timing.total_time_ns = (tv1.tv_sec * 1000000000ull + tv1.tv_usec * 1000ull) - (tv0.tv_sec * 1000000000ull + tv0.tv_usec * 1000ull);
  //counter->stop();
  //printf("ocl_%s PAGE_FAULTS_COUNT: %lld\n", (single_core) ? "single" : "all",  counter->value());
  //std::cin.get();
	//delete counter;
  //#################################################

	return (unsigned long long) timing.total_time_ns;
}
