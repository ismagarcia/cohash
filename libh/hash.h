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

