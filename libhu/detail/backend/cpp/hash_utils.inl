#include <libhu/detail/backend/cpp/detail/hash_utils.h>

namespace libhu
{
namespace detail
{
namespace backend
{
namespace cpp
{

template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2,
         typename T_RAND_ACCESS_ITERATOR3,
         typename T_HASH_FUNCTOR>
  void access(T_RAND_ACCESS_ITERATOR1 search_keys_begin,
              T_RAND_ACCESS_ITERATOR1 search_keys_end,
              T_RAND_ACCESS_ITERATOR2 hash_table_begin,
              T_RAND_ACCESS_ITERATOR2 hash_table_end,
              T_RAND_ACCESS_ITERATOR3 output_values_begin,
              T_HASH_FUNCTOR          hf,
              bool                    constrained_hash_access,
              libhu::U32              max_age)
{
  libhu::detail::backend::cpp::detail::access(search_keys_begin, search_keys_end, hash_table_begin, hash_table_end, output_values_begin, hf, constrained_hash_access, max_age);
}

} // end namespace cpp
} // end namespace backend
} // end namespace detail
} // end namespace libhu

