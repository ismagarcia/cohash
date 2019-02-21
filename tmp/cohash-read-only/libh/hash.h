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

#pragma once

#include <thrust/detail/config.h>
#include <libhu/hash_utils.h>

// ------------------------------------------------------------------

namespace libh
{

template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2,
         typename T_HASH_FUNCTOR>
  void hash(T_RAND_ACCESS_ITERATOR1 keys_begin,
            T_RAND_ACCESS_ITERATOR1 keys_end,
            T_RAND_ACCESS_ITERATOR2 hash_table_begin,
            T_RAND_ACCESS_ITERATOR2 hash_table_end,
            T_HASH_FUNCTOR          hf);

template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2,
         typename T_HASH_FUNCTOR>
  void hash(T_RAND_ACCESS_ITERATOR1 keys_begin,
            T_RAND_ACCESS_ITERATOR1 keys_end,
            T_RAND_ACCESS_ITERATOR2 hash_table_begin,
            T_RAND_ACCESS_ITERATOR2 hash_table_end,
            T_HASH_FUNCTOR          hf,
            libhu::U32             &max_age);

template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2,
         typename T_HASH_FUNCTOR>
  void hash(T_RAND_ACCESS_ITERATOR1 keys_begin,
            T_RAND_ACCESS_ITERATOR1 keys_end,
            T_RAND_ACCESS_ITERATOR2 hash_table_begin,
            T_RAND_ACCESS_ITERATOR2 hash_table_end,
            T_HASH_FUNCTOR          hf,
            bool                    constrained_hash_access);

template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2,
         typename T_HASH_FUNCTOR>
  void hash(T_RAND_ACCESS_ITERATOR1 keys_begin,
            T_RAND_ACCESS_ITERATOR1 keys_end,
            T_RAND_ACCESS_ITERATOR2 hash_table_begin,
            T_RAND_ACCESS_ITERATOR2 hash_table_end,
            T_HASH_FUNCTOR          hf,
            bool                    constrained_hash_access,
            libhu::U32             &max_age);

///////////////
// Key Value //
///////////////

template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2,
         typename T_RAND_ACCESS_ITERATOR3,
         typename T_RAND_ACCESS_ITERATOR4,
         typename T_HASH_FUNCTOR>
  void hash_by_key(T_RAND_ACCESS_ITERATOR1 keys_begin,
                   T_RAND_ACCESS_ITERATOR1 keys_end,
                   T_RAND_ACCESS_ITERATOR3 values_begin,
                   T_RAND_ACCESS_ITERATOR4 hash_table_begin,
                   T_RAND_ACCESS_ITERATOR4 hash_table_end,
                   T_HASH_FUNCTOR hf);

template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2,
         typename T_RAND_ACCESS_ITERATOR3,
         typename T_RAND_ACCESS_ITERATOR4,
         typename T_HASH_FUNCTOR>
  void hash_by_key(T_RAND_ACCESS_ITERATOR1 keys_begin,
                   T_RAND_ACCESS_ITERATOR1 keys_end,
                   T_RAND_ACCESS_ITERATOR3 values_begin,
                   T_RAND_ACCESS_ITERATOR4 hash_table_begin,
                   T_RAND_ACCESS_ITERATOR4 hash_table_end,
                   T_HASH_FUNCTOR          hf,
                   libhu::U32             &max_age);

template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2,
         typename T_RAND_ACCESS_ITERATOR3,
         typename T_RAND_ACCESS_ITERATOR4,
         typename T_HASH_FUNCTOR>
  void hash_by_key(T_RAND_ACCESS_ITERATOR1 keys_begin,
                   T_RAND_ACCESS_ITERATOR1 keys_end,
                   T_RAND_ACCESS_ITERATOR3 values_begin,
                   T_RAND_ACCESS_ITERATOR4 hash_table_begin,
                   T_RAND_ACCESS_ITERATOR4 hash_table_end,
                   bool                    constrained_hash_access,
                   T_HASH_FUNCTOR          hf);

template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2,
         typename T_RAND_ACCESS_ITERATOR3,
         typename T_RAND_ACCESS_ITERATOR4,
         typename T_HASH_FUNCTOR>
  void hash_by_key(T_RAND_ACCESS_ITERATOR1 keys_begin,
                   T_RAND_ACCESS_ITERATOR1 keys_end,
                   T_RAND_ACCESS_ITERATOR3 values_begin,
                   T_RAND_ACCESS_ITERATOR4 hash_table_begin,
                   T_RAND_ACCESS_ITERATOR4 hash_table_end,
                   T_HASH_FUNCTOR          hf,
                   bool                    constrained_hash_access,
                   libhu::U32             &max_age);

} // end namespace libh

#include <libh/detail/hash.inl>

