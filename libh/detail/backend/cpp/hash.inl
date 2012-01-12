#include <libh/detail/backend/cpp/detail/hash.h>

namespace libh
{
namespace detail
{
namespace backend
{
namespace cpp
{

template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2,
         typename T_HASH_FUNCTOR>
  void hash(T_RAND_ACCESS_ITERATOR1 keys_begin,
                   T_RAND_ACCESS_ITERATOR1 keys_end,
                   T_RAND_ACCESS_ITERATOR2 hash_table_begin,
                   T_RAND_ACCESS_ITERATOR2 hash_table_end,
                   T_HASH_FUNCTOR          hf,
                   bool                    constrained_hash_access,
                   libhu::U32             &max_age)
{
  libh::detail::backend::cpp::detail::hash(keys_begin, keys_end, hash_table_begin, hash_table_end, hf, constrained_hash_access, max_age);
}

template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2,
         typename T_RAND_ACCESS_ITERATOR3,
         typename T_HASH_FUNCTOR>
  void hash_by_key(T_RAND_ACCESS_ITERATOR1 keys_begin,
                   T_RAND_ACCESS_ITERATOR1 keys_end,
                   T_RAND_ACCESS_ITERATOR2 values_begin,
                   T_RAND_ACCESS_ITERATOR3 hash_table_begin,
                   T_RAND_ACCESS_ITERATOR3 hash_table_end,
                   T_HASH_FUNCTOR          hf,
                   bool                    constrained_hash_access,
                   libhu::U32             &max_age)
{
  libh::detail::backend::cpp::detail::hash_by_key(keys_begin, keys_end, values_begin, hash_table_begin, hash_table_end, hf, constrained_hash_access, max_age);
}

} // end namespace cpp
} // end namespace backend
} // end namespace detail
} // end namespace libh

