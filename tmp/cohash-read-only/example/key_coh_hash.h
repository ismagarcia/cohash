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

#ifndef KEY_COH_HASH_H_
#define KEY_COH_HASH_H_

#include <libhu/hash_utils.h>

// ------------------------------------------------------------------

#if (ENABLE_DEVICE_OMP_COMPUTING)
  __constant__ unsigned int offsets_k_coh[16] = OFFSETS_TABLE_16;
#elif (ENABLE_DEVICE_CUDA_COMPUTING)
  __constant__ unsigned int offsets_k_coh[16] = OFFSETS_TABLE_16;
#elif (ENABLE_DEVICE_OPENCL_COMPUTING)
  __constant__ unsigned int offsets_k_coh[16] = OFFSETS_TABLE_16;
#endif

class key_coh_hash_functor : libhu::key_hash_functor
{
public:

  key_coh_hash_functor()
  {
  }
  
typedef libhu::U32 T_KEY;
typedef libhu::U32 T_AGE;
typedef libhu::U32 T_MAX_AGE;
typedef libhu::U32 T_LOC;
typedef libhu::U32 T_HASH_TABLE;

static const libhu::U32 DEFAULT_GROUP_SIZE;

static const libhu::U32 KEY_TYPE_BITS;
static const libhu::U32 KEY_TYPE_MASK;
static const libhu::U32 PACKED_KEY_TYPE_MASK;
static const libhu::U32 KEY_TYPE_RANGE;
static const libhu::U32 UNDEFINED_KEY;
static const libhu::U32 PACKED_UNDEFINED_KEY;

static const libhu::U32 KEY_TYPE_AGE_MASK;
static const libhu::U32 KEY_TYPE_AGE_BITS;
static const libhu::U32 KEY_TYPE_INIT_AGE;
static const libhu::U32 KEY_TYPE_NULL_AGE;
static const libhu::U32 KEY_TYPE_MAX_AGE;
static const libhu::U32 KEY_TYPE_MAX_AGE_MASK;
static const libhu::U32 KEY_TYPE_MAX_AGE_BITS;

static const libhu::U32 HTABLE_ID;
static const libhu::U32 NOP_MODE_TRUE;
static const libhu::U32 NOP_MODE_FALSE;

libhu::UPTR hash_tableUPtr;
libhu::U32* max_tableUPtr;
libhu::U32  hash_table_size;

__inline__ HOST DEVICE T_LOC        WRAP(T_LOC A, T_LOC B)                          { return ((A) % (B)); }

__inline__ HOST DEVICE T_KEY        GET_KEY_POS(T_HASH_TABLE k)                     { return ((k) & KEY_TYPE_MASK); }
__inline__ HOST DEVICE T_KEY        GET_KEY_ATTACH_ID(T_HASH_TABLE k)               { return ((k) & KEY_TYPE_MASK); }
__inline__ HOST DEVICE T_AGE        GET_KEY_AGE(T_HASH_TABLE k)                     { return ((k) >> KEY_TYPE_BITS); }
__inline__ HOST DEVICE T_MAX_AGE    GET_KEY_MAX_AGE(T_HASH_TABLE k)                 { return ((k) >> KEY_TYPE_BITS); }
__inline__ HOST DEVICE T_HASH_TABLE PACK_KEY_POS(T_KEY p)                           { return ((p) & KEY_TYPE_MASK); }
__inline__ HOST DEVICE T_HASH_TABLE PACK_KEY_POS_AND_AGE(T_KEY p, T_AGE a)          { return (((a << KEY_TYPE_BITS)) + (p & KEY_TYPE_MASK)); }
__inline__ HOST DEVICE T_HASH_TABLE PACK_KEY_POS_AND_MAX_AGE(T_KEY p, T_MAX_AGE m)  { return (((m << KEY_TYPE_BITS)) + (p & KEY_TYPE_MASK)); }


// Hash function
__inline__ HOST DEVICE T_LOC h(T_KEY K, T_AGE AGE, libhu::U32 HSZ)
{
#if (ENABLE_HOST_COMPUTING)
  static unsigned int offsets_k_coh[16] = OFFSETS_TABLE_16;
#endif

  return WRAP((offsets_k_coh[AGE] + K), HSZ);
}

// Max. age operator to update hash_table
DEVICE 
void operator()(T_HASH_TABLE& t)
{
  libhu::U32 i = (((libhu::UPTR)thrust::raw_pointer_cast(&t)) - ((libhu::UPTR)hash_tableUPtr)) / (sizeof(T_HASH_TABLE));
  if (t != PACKED_UNDEFINED_KEY)
  {
    t = PACK_KEY_POS_AND_MAX_AGE(GET_KEY_POS(t), max_tableUPtr[i]);
  }
}

template<typename T_KEY,
         typename T_HASH_TABLE,
         typename T_MAX_AGE,
         typename T_HASH_FUNCTOR,
         typename T_MAX_AGE_COMPUTATION_FUNCTOR>
__inline__ HOST DEVICE  
void hash_kernel(libhu::U32            keys_size,
                 libhu::U32            hash_table_size,
                 T_KEY          keys[],
                 T_HASH_TABLE   hash_table[],
                 T_MAX_AGE      max_table[],
                 T_HASH_FUNCTOR hf,
                 T_MAX_AGE_COMPUTATION_FUNCTOR maf)
{

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
  // initialize variables
  libhu::U32 LOC;
  libhu::U32 ROOT_LOC;
  libhu::U8 AGE            = KEY_TYPE_NULL_AGE;  
  T_HASH_TABLE EVICTED_PKEY;
  T_HASH_TABLE PKEY = (GTID < keys_size) ? PACK_KEY_POS_AND_MAX_AGE(keys[ (GTID) ], KEY_TYPE_INIT_AGE) : PACKED_UNDEFINED_KEY;
  AGE               = (GTID < keys_size) ? KEY_TYPE_NULL_AGE : KEY_TYPE_MAX_AGE;

  while (AGE < KEY_TYPE_MAX_AGE)
  {
      LOC  = h(GET_KEY_POS(PKEY), AGE, hash_table_size);
      EVICTED_PKEY = libhu::atomicMaxU32(&hash_table[ (LOC) ], PKEY );

      if (EVICTED_PKEY < PKEY)
      { 

        maf.update_max_age(hash_table_size, PKEY, AGE, max_table, hf);

        if (GET_KEY_AGE(EVICTED_PKEY) > 0u)
        { 
          PKEY       = EVICTED_PKEY;
          AGE        = GET_KEY_AGE(EVICTED_PKEY); 
        }
        else 
        {
          break;
       }
    }
    else
    {
      AGE++;
      PKEY     = PACK_KEY_POS_AND_MAX_AGE(GET_KEY_POS(PKEY), AGE);
    }
  }
  }
}
    
};

const libhu::U32 key_coh_hash_functor::DEFAULT_GROUP_SIZE       = 192u;

const libhu::U32 key_coh_hash_functor::KEY_TYPE_BITS            = 28u;
const libhu::U32 key_coh_hash_functor::KEY_TYPE_MASK            = libhu::U32( libhu::U64((1ull) << KEY_TYPE_BITS) - 1u );
const libhu::U32 key_coh_hash_functor::PACKED_KEY_TYPE_MASK     = libhu::U32( libhu::U64((1ull) << KEY_TYPE_BITS) - 1u );
const libhu::U32 key_coh_hash_functor::KEY_TYPE_RANGE           = libhu::U32( libhu::U64((1ull) << KEY_TYPE_BITS) - 2u );
const libhu::U32 key_coh_hash_functor::UNDEFINED_KEY            = libhu::U32( libhu::U64((1ull) << KEY_TYPE_BITS) - 1u );
const libhu::U32 key_coh_hash_functor::PACKED_UNDEFINED_KEY     = libhu::U32( libhu::U64((1ull) << KEY_TYPE_BITS) - 1u );

const libhu::U32 key_coh_hash_functor::KEY_TYPE_AGE_MASK        = 15u;
const libhu::U32 key_coh_hash_functor::KEY_TYPE_AGE_BITS        = 4u;
const libhu::U32 key_coh_hash_functor::KEY_TYPE_INIT_AGE        = 1u;
const libhu::U32 key_coh_hash_functor::KEY_TYPE_NULL_AGE        = 0u;
const libhu::U32 key_coh_hash_functor::KEY_TYPE_MAX_AGE         = 16u;
const libhu::U32 key_coh_hash_functor::KEY_TYPE_MAX_AGE_MASK    = 4u;
const libhu::U32 key_coh_hash_functor::KEY_TYPE_MAX_AGE_BITS    = 4u;

const libhu::U32 key_coh_hash_functor::HTABLE_ID                = 0u;
const libhu::U32 key_coh_hash_functor::NOP_MODE_TRUE            = 255u;
const libhu::U32 key_coh_hash_functor::NOP_MODE_FALSE           = 0u;

#endif
