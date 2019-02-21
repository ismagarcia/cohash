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

#ifndef KEY_VALUE_COH_HASH_H_
#define KEY_VALUE_COH_HASH_H_
#include <iostream>
#include <libhu/hash_utils.h>

#if (ENABLE_GATOR)
#include <streamline_annotate.h>
#endif

#if (ENABLE_NVPA)
#include <NvPAGPUEventSrcAPI.h>
#include <NvPAUtil.h>
NVPA_EXTERN_GLOBALS;
#endif

#if (ENABLE_PERF)
#include <signal.h>
#endif
// ------------------------------------------------------------------

#if (ENABLE_DEVICE_OMP_COMPUTING)
  __constant__ unsigned int offsets_kv_coh[16] = OFFSETS_TABLE_16;
#elif (ENABLE_DEVICE_CUDA_COMPUTING)
  __constant__ unsigned int offsets_kv_coh[16] = OFFSETS_TABLE_16;
#elif (ENABLE_DEVICE_OPENCL_COMPUTING)
  __constant__ unsigned int offsets_kv_coh[16] = OFFSETS_TABLE_16;
#endif

class key_value_coh_hash_functor : libhu::key_hash_functor
{

public:

typedef libhu::U32 T_KEY;
typedef libhu::U32 T_VALUE;
typedef libhu::U32 T_ATTACH_ID;
typedef libhu::U32 T_AGE;
typedef libhu::U32 T_MAX_AGE;
	typedef libhu::U32 T_LOC;
typedef libhu::U64 T_HASH_TABLE;

//static const libhu::U32 DEFAULT_GROUP_SIZE            = 192u;
//
//static const libhu::U32 KEY_TYPE_BITS                 = 32u;
//static const libhu::U32 KEY_TYPE_MASK                 = libhu::U32( libhu::U64((1ull) << KEY_TYPE_BITS) - 1ul );
//static const libhu::U32 PACKED_KEY_TYPE_MASK          = libhu::U32( libhu::U64((1ull) << KEY_TYPE_BITS) - 1ul );
//static const libhu::U32 KEY_TYPE_RANGE                = libhu::U32( libhu::U64((1ull) << KEY_TYPE_BITS) - 2ul );
//static const libhu::U32 UNDEFINED_KEY                 = libhu::U32( libhu::U64((1ull) << KEY_TYPE_BITS) - 1ul );
//static const libhu::U32 UNDEFINED_KEY;
//static const libhu::U32 PACKED_UNDEFINED_KEY          = libhu::U32( libhu::U64((1ull) << KEY_TYPE_BITS) - 1ul );
//
//static const libhu::U32 ATTACH_ID_TYPE_BITS           = 28u;
//static const libhu::U32 ATTACH_ID_TYPE_MASK           = libhu::U32( libhu::U64((1ull) << ATTACH_ID_TYPE_BITS) - 1ul );
//static const libhu::U32 UNDEFINED_ATTACH_ID           = libhu::U32( libhu::U64((1ull) << ATTACH_ID_TYPE_BITS) - 1ul );
//static const libhu::U32 MAX_ATTACH_ID_TYPE            = libhu::U32( libhu::U64((1ull) << ATTACH_ID_TYPE_BITS) - 2ul );
//
//static const libhu::U32 KEY_TYPE_AGE_MASK             = 15u;
//static const libhu::U32 KEY_TYPE_AGE_BITS             = 4u;
//static const libhu::U32 INIT_KEY_TYPE_AGE             = 1u;
//static const libhu::U32 KEY_TYPE_NULL_AGE             = 0u;
//
//static const libhu::U32 KEY_TYPE_MAX_AGE              = 16u;
//
//static const libhu::U32 KEY_TYPE_MAX_AGE_MASK         = 4u;
//
//static const libhu::U32 KEY_TYPE_MAX_AGE_BITS         = 4u;
//
//static const libhu::U32 HTABLE_ID                     = 0u;


static const libhu::U32 DEFAULT_GROUP_SIZE;

static const libhu::U32 KEY_TYPE_BITS;
static const libhu::U32 KEY_TYPE_MASK;
static const libhu::U32 PACKED_KEY_TYPE_MASK;
static const libhu::U32 KEY_TYPE_RANGE;
static const libhu::U32 UNDEFINED_KEY;
static const libhu::U32 PACKED_UNDEFINED_KEY;

static const libhu::U32 ATTACH_ID_TYPE_BITS;
static const libhu::U32 ATTACH_ID_TYPE_MASK;
static const libhu::U32 UNDEFINED_ATTACH_ID;
static const libhu::U32 MAX_ATTACH_ID_TYPE;

static const libhu::U32 KEY_TYPE_AGE_MASK;
static const libhu::U32 KEY_TYPE_AGE_BITS;
static const libhu::U32 INIT_KEY_TYPE_AGE;
static const libhu::U32 KEY_TYPE_NULL_AGE;

static const libhu::U32 KEY_TYPE_MAX_AGE;

static const libhu::U32 KEY_TYPE_MAX_AGE_MASK;

static const libhu::U32 KEY_TYPE_MAX_AGE_BITS;

static const libhu::U32 HTABLE_ID;


libhu::UPTR hash_tableUPtr;
libhu::U32* max_tableUPtr;
libhu::U32  hash_table_size;

__inline__ HOST DEVICE T_LOC        WRAP(T_LOC A, T_LOC B)                                              { return ((A) % (B)); }
__inline__ HOST DEVICE T_AGE        GET_KEY_AGE(T_HASH_TABLE k)                                         { return ((k) >> (ATTACH_ID_TYPE_BITS + KEY_TYPE_BITS)); }
__inline__ HOST DEVICE T_MAX_AGE    GET_KEY_MAX_AGE(T_HASH_TABLE k)                                     { return ((k) >> (ATTACH_ID_TYPE_BITS + KEY_TYPE_BITS)); }

HOST __inline__ DEVICE T_KEY        GET_KEY_POS(T_HASH_TABLE k)                                                  { return ((k) & KEY_TYPE_MASK); }
HOST __inline__ DEVICE T_VALUE      GET_KEY_ATTACH_ID(T_HASH_TABLE k)                                            { return ((k) >> (KEY_TYPE_BITS)) & ATTACH_ID_TYPE_MASK; }
HOST __inline__ DEVICE T_HASH_TABLE PACK_KEY_POS(T_KEY p)                                                        { return ((p) & KEY_TYPE_MASK); }
HOST __inline__ DEVICE T_HASH_TABLE PACK_KEY_POS_AND_ATTACH_ID_AND_AGE(T_KEY p, T_ATTACH_ID i, T_AGE a)          { return T_HASH_TABLE(((T_HASH_TABLE(a) << (ATTACH_ID_TYPE_BITS + KEY_TYPE_BITS))) + ((T_HASH_TABLE(i) & ATTACH_ID_TYPE_MASK) << (KEY_TYPE_BITS)) + T_HASH_TABLE(p & KEY_TYPE_MASK)); }
HOST __inline__ DEVICE T_HASH_TABLE PACK_KEY_POS_AND_ATTACH_ID_AND_MAX_AGE(T_KEY p, T_ATTACH_ID i, T_MAX_AGE m)  { return T_HASH_TABLE(((T_HASH_TABLE(m) << (ATTACH_ID_TYPE_BITS + KEY_TYPE_BITS))) + ((T_HASH_TABLE(i) & ATTACH_ID_TYPE_MASK) << (KEY_TYPE_BITS)) + T_HASH_TABLE(p & KEY_TYPE_MASK)); }

// Hash function
__inline__ HOST DEVICE T_LOC h(T_KEY K, T_AGE AGE, libhu::U32 HSZ)
{
#if (ENABLE_HOST_COMPUTING)
  static unsigned int offsets_kv_coh[16] = OFFSETS_TABLE_16;
#endif

  return WRAP((offsets_kv_coh[AGE] + K ), HSZ);
}

// Max. age operator to update hash_table
__inline__ HOST  DEVICE
void operator()(T_HASH_TABLE& t)
{
  libhu::U32 i = (((libhu::UPTR)thrust::raw_pointer_cast(&t)) - ((libhu::UPTR)hash_tableUPtr)) / (sizeof(T_HASH_TABLE));
  if (t != PACKED_UNDEFINED_KEY)
  {
    t = PACK_KEY_POS_AND_ATTACH_ID_AND_MAX_AGE(GET_KEY_POS(t), GET_KEY_ATTACH_ID(t), max_tableUPtr[i]);
  }
}

template<typename T_KEY,
         typename T_VALUE,
         typename T_HASH_TABLE,
         typename T_MAX_AGE,
         typename T_HASH_FUNCTOR,
         typename T_MAX_AGE_COMPUTATION_FUNCTOR>
__inline__ HOST DEVICE

void hash_by_key_kernel(libhu::U32            keys_size,
                        libhu::U32            hash_table_size,
                        T_KEY                 keys[],
                        T_VALUE               values[],
                        T_HASH_TABLE          hash_table[],
                        T_MAX_AGE             max_table[],
                        T_HASH_FUNCTOR        hf,
                        T_MAX_AGE_COMPUTATION_FUNCTOR maf)
{

#if (ENABLE_PERF)
char cmd[1024];
sprintf(cmd, "/media/ARM_DEV/work/dev/experimental/cohash-read-only/example/perf stat -a -C 0,1,2,3,4 -g -e CCI/config=0x83,name=S4_A7_Shareable_Transactions/ -e CCI/config=0x63,name=S3_A15_Shareable_Transactions/ -e CCI/config=0xd4,name=M1_retries/ -e CCI/config=0xff,name=cci_cycles/ -p %d &", getpid());
system(cmd);
#endif

#if (ENABLE_GATOR)
  ANNOTATE_MARKER_COLOR_STR(ANNOTATE_BLUE, "coh_hash_build_insert_pre");
#endif

#if (ENABLE_DEVICE_OMP_COMPUTING)

  #if (OMP_CUSTOM_OPTIONS_ON_RUNTIME)
  omp_set_num_threads(OMP_CUSTOM_NUM_THREADS);
  omp_set_schedule(omp_sched_dynamic,OMP_CUSTOM_CHUNK_SIZE);
  #pragma omp parallel for num_threads(OMP_CUSTOM_NUM_THREADS),schedule(runtime)
  for (libhu::S32 GTID = 0; GTID < keys_size; GTID++)
  #else
  #pragma omp parallel for num_threads(OMP_CUSTOM_NUM_THREADS),schedule(dynamic, OMP_CUSTOM_CHUNK_SIZE)
  for (libhu::S32 GTID = 0; GTID < keys_size; GTID++)
  #endif

#endif
#if (ENABLE_HOST_COMPUTING)
  for (libhu::S32 GTID = 0; GTID < keys_size; GTID++)
#endif
  {

    //printf ("thread_id: %d\n", (int) syscall(SYS_gettid));

#if (!DISABLE_NVPA_KERNEL_EVEN_MEMORY_DECLARATION)
    NVPA_KERNEL_START_EVENT(cudainternalBuildHash);
    //KERNEL_TSTAMP_EVENT(0);
#endif

    libhu::U32 LOC;
    libhu::U8 AGE     = KEY_TYPE_NULL_AGE;
    T_HASH_TABLE EVICTED_PKEY;
    T_HASH_TABLE PKEY = (GTID < keys_size) ? 
                                 PACK_KEY_POS_AND_ATTACH_ID_AND_AGE(keys[ (GTID) ], values[ (GTID) ], INIT_KEY_TYPE_AGE) : 
                                 PACK_KEY_POS_AND_ATTACH_ID_AND_AGE(UNDEFINED_KEY, UNDEFINED_ATTACH_ID, KEY_TYPE_NULL_AGE);
    AGE               = (GTID < keys_size) ? KEY_TYPE_NULL_AGE : KEY_TYPE_MAX_AGE;

    while (AGE < KEY_TYPE_MAX_AGE)
    {
      LOC  = h(GET_KEY_POS(PKEY), AGE, hash_table_size);

      //KERNEL_TSTAMP_EVENT(1);

      EVICTED_PKEY = libhu::atomicMaxU64(&hash_table[ (LOC) ], PKEY );

      //KERNEL_TSTAMP_EVENT(2);

      if (EVICTED_PKEY < PKEY)
      { 

        maf.update_max_age(hash_table_size, PKEY, AGE, max_table, hf);

        if (GET_KEY_AGE(EVICTED_PKEY) > 0u)
        { 
          PKEY = EVICTED_PKEY;
          AGE  = GET_KEY_AGE(EVICTED_PKEY); 
        }
        else 
        {

#if (!DISABLE_NVPA_KERNEL_EVEN_MEMORY_DECLARATION)
        //KERNEL_TSTAMP_EVENT(3);
        NVPA_KERNEL_STOP_EVENT(cudainternalBuildHash);
#endif

          break;
        }
      }
      else
      {
        AGE++;
        PKEY = PACK_KEY_POS_AND_ATTACH_ID_AND_AGE(GET_KEY_POS(PKEY), GET_KEY_ATTACH_ID(PKEY), AGE);
      }
    }

#if (!DISABLE_NVPA_KERNEL_EVEN_MEMORY_DECLARATION)
    //KERNEL_TSTAMP_EVENT(3);
    NVPA_KERNEL_STOP_EVENT(cudainternalBuildHash);
#endif

  }

#if (ENABLE_GATOR)
  ANNOTATE_MARKER_COLOR_STR(ANNOTATE_BLUE, "coh_hash_build_insert_pos");
#endif

#if (ENABLE_PERF)
kill(0, SIGINT);
#endif

}
  
};

const libhu::U32 key_value_coh_hash_functor::DEFAULT_GROUP_SIZE            = 192u;

const libhu::U32 key_value_coh_hash_functor::KEY_TYPE_BITS                 = 32u;
const libhu::U32 key_value_coh_hash_functor::KEY_TYPE_MASK                 = libhu::U32( libhu::U64((1ull) << key_value_coh_hash_functor::KEY_TYPE_BITS) - 1ul );
const libhu::U32 key_value_coh_hash_functor::PACKED_KEY_TYPE_MASK          = libhu::U32( libhu::U64((1ull) << key_value_coh_hash_functor::KEY_TYPE_BITS) - 1ul );
const libhu::U32 key_value_coh_hash_functor::KEY_TYPE_RANGE                = libhu::U32( libhu::U64((1ull) << key_value_coh_hash_functor::KEY_TYPE_BITS) - 2ul );
const libhu::U32 key_value_coh_hash_functor::UNDEFINED_KEY                 = libhu::U32( libhu::U64((1ull) << key_value_coh_hash_functor::KEY_TYPE_BITS) - 1ul );
const libhu::U32 key_value_coh_hash_functor::PACKED_UNDEFINED_KEY          = libhu::U32( libhu::U64((1ull) << key_value_coh_hash_functor::KEY_TYPE_BITS) - 1ul );

const libhu::U32 key_value_coh_hash_functor::ATTACH_ID_TYPE_BITS           = 28u;
const libhu::U32 key_value_coh_hash_functor::ATTACH_ID_TYPE_MASK           = libhu::U32( libhu::U64((1ull) << key_value_coh_hash_functor::ATTACH_ID_TYPE_BITS) - 1ul );
const libhu::U32 key_value_coh_hash_functor::UNDEFINED_ATTACH_ID           = libhu::U32( libhu::U64((1ull) << key_value_coh_hash_functor::ATTACH_ID_TYPE_BITS) - 1ul );
const libhu::U32 key_value_coh_hash_functor::MAX_ATTACH_ID_TYPE            = libhu::U32( libhu::U64((1ull) << key_value_coh_hash_functor::ATTACH_ID_TYPE_BITS) - 2ul );

const libhu::U32 key_value_coh_hash_functor::KEY_TYPE_AGE_MASK             = 15u;
const libhu::U32 key_value_coh_hash_functor::KEY_TYPE_AGE_BITS             = 4u;
const libhu::U32 key_value_coh_hash_functor::INIT_KEY_TYPE_AGE             = 1u;
const libhu::U32 key_value_coh_hash_functor::KEY_TYPE_NULL_AGE             = 0u;

const libhu::U32 key_value_coh_hash_functor::KEY_TYPE_MAX_AGE              = 16u;

const libhu::U32 key_value_coh_hash_functor::KEY_TYPE_MAX_AGE_MASK         = 4u;

const libhu::U32 key_value_coh_hash_functor::KEY_TYPE_MAX_AGE_BITS         = 4u;

const libhu::U32 key_value_coh_hash_functor::HTABLE_ID                     = 0u;


#endif
