#pragma once

namespace libh
{
namespace detail
{
namespace backend
{
namespace cuda
{
namespace detail
{

template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2, 
         typename T_HASH_FUNCTOR,
         typename hash_access_tag>
void hash(T_RAND_ACCESS_ITERATOR1 keys_begin,
          T_RAND_ACCESS_ITERATOR1 keys_end,
          T_RAND_ACCESS_ITERATOR2 hash_table_begin,
          T_RAND_ACCESS_ITERATOR2 hash_table_end,
          T_HASH_FUNCTOR          hf,
          bool                    constrained_hash_access,
          libhu::U32             &max_age);
    
template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2,
         typename T_RAND_ACCESS_ITERATOR3,
         typename T_HASH_FUNCTOR,
         typename hash_access_tag>
void hash_by_key(T_RAND_ACCESS_ITERATOR1 keys_begin,
                 T_RAND_ACCESS_ITERATOR1 keys_end,
                 T_RAND_ACCESS_ITERATOR2 values_begin,
                 T_RAND_ACCESS_ITERATOR3 hash_table_begin,
                 T_RAND_ACCESS_ITERATOR3 hash_table_end,
                 T_HASH_FUNCTOR          hf,
                 bool                    constrained_hash_access,
                 libhu::U32             &max_age);

} // end namespace detail
} // end namespace cuda
} // end namespace backend
} // end namespace detail
} // end namespace libh

#include "hash.inl"

