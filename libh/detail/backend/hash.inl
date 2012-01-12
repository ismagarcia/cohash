#include <thrust/copy.h>
#include <thrust/detail/trivial_sequence.h>

#include <thrust/iterator/iterator_traits.h>
#include <thrust/iterator/detail/minimum_space.h>

#include <libh/detail/backend/generic/hash.h>
#include <libh/detail/backend/cpp/hash.h>
#include <libh/detail/backend/cuda/hash.h>

namespace libh
{
namespace detail
{
namespace backend
{
namespace dispatch
{


template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2,
         typename T_HASH_FUNCTOR,
         typename Backend>
  void hash(T_RAND_ACCESS_ITERATOR1 keys_begin,
            T_RAND_ACCESS_ITERATOR1 keys_end,
            T_RAND_ACCESS_ITERATOR2 hash_table_begin,
            T_RAND_ACCESS_ITERATOR2 hash_table_end,
            T_HASH_FUNCTOR          hf,
            bool                    constrained_hash_access,
            libhu::U32             &max_age,
            Backend)
{
  libh::detail::backend::generic::hash(keys_begin, keys_end, hash_table_begin, hash_table_end, hf, constrained_hash_access, max_age);
} // end hash()

template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2,
         typename T_HASH_FUNCTOR>
  void hash(T_RAND_ACCESS_ITERATOR1 keys_begin,
            T_RAND_ACCESS_ITERATOR1 keys_end,
            T_RAND_ACCESS_ITERATOR2 hash_table_begin,
            T_RAND_ACCESS_ITERATOR2 hash_table_end,
            T_HASH_FUNCTOR          hf,
            bool                    constrained_hash_access,
            libhu::U32             &max_age,
            thrust::host_space_tag)
{
  //if (libhu::checkTargetISAIsSupported())
  //{
  //  std::cerr << "libh::detail::backend::ispc::hash(...)" << std::endl;
  //  libh::detail::backend::ispc::hash(keys_begin, keys_end, hash_table_begin, hash_table_end, hf, constrained_hash_access, max_age);
  //}
  //else
  {
    libh::detail::backend::cpp::hash(keys_begin, keys_end, hash_table_begin, hash_table_end, hf, constrained_hash_access, max_age);
  }
}

template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2,
         typename T_HASH_FUNCTOR>
  void hash(T_RAND_ACCESS_ITERATOR1 keys_begin,
            T_RAND_ACCESS_ITERATOR1 keys_end,
            T_RAND_ACCESS_ITERATOR2 hash_table_begin,
            T_RAND_ACCESS_ITERATOR2 hash_table_end,
            T_HASH_FUNCTOR          hf,
            bool                    constrained_hash_access,
            libhu::U32             &max_age,
            thrust::detail::cuda_device_space_tag)
{
  // ensure sequence has trivial iterators
  // XXX this prologue belongs somewhere else
  thrust::detail::trivial_sequence<T_RAND_ACCESS_ITERATOR1> keys(keys_begin, keys_end);
  thrust::detail::trivial_sequence<T_RAND_ACCESS_ITERATOR2> hash_table(hash_table_begin, hash_table_end);

  libh::detail::backend::cuda::hash(keys.begin(), keys.end(), hash_table.begin(), hash_table.end(), hf, constrained_hash_access, max_age);

  // copy results back, if necessary
  // XXX this epilogue belongs somewhere else
  if(!thrust::detail::is_trivial_iterator<T_RAND_ACCESS_ITERATOR1>::value)
  {
    thrust::copy(hash_table.begin(), hash_table.end(), hash_table_begin);
  }
}

template<typename T_RAND_ACCESS_ITERATOR1,
         typename T_RAND_ACCESS_ITERATOR2,
         typename T_RAND_ACCESS_ITERATOR3,        
         typename T_HASH_FUNCTOR,
         typename Backend>
  void hash_by_key(T_RAND_ACCESS_ITERATOR1 keys_begin,
                   T_RAND_ACCESS_ITERATOR1 keys_end,
                   T_RAND_ACCESS_ITERATOR2 values_begin,
                   T_RAND_ACCESS_ITERATOR3 hash_table_begin,
                   T_RAND_ACCESS_ITERATOR3 hash_table_end,
                   T_HASH_FUNCTOR          hf,
                   bool                    constrained_hash_access,
                   libhu::U32             &max_age,
                   Backend)
{
  libh::detail::backend::generic::hash_by_key(keys_begin, keys_end, values_begin, hash_table_begin, hash_table_end, hf, constrained_hash_access, max_age);
} // end hash_by_key()

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
                   libhu::U32             &max_age,
                   thrust::host_space_tag)
{
  //if (libhu::checkTargetISAIsSupported())
  //{
  //  std::cerr << "libh::detail::backend::ispc::hash_by_key(...)" << std::endl;
  //  libh::detail::backend::ispc::hash_by_key(keys_begin, keys_end, values_begin, hash_table_begin, hash_table_end, hf, constrained_hash_access, max_age);
  //}
  //else
  {
    libh::detail::backend::cpp::hash_by_key(keys_begin, keys_end, values_begin, hash_table_begin, hash_table_end, hf, constrained_hash_access, max_age);
  }
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
                   libhu::U32             &max_age,
                   thrust::detail::cuda_device_space_tag)
{
  // ensure sequences have trivial iterators
  // XXX this prologue belongs somewhere else
  T_RAND_ACCESS_ITERATOR2 values_end = values_begin + (keys_end - keys_begin);
  thrust::detail::trivial_sequence<T_RAND_ACCESS_ITERATOR1> keys(keys_begin, keys_end);
  thrust::detail::trivial_sequence<T_RAND_ACCESS_ITERATOR2> values(values_begin, values_end);
  thrust::detail::trivial_sequence<T_RAND_ACCESS_ITERATOR3> hash_table(hash_table_begin, hash_table_end);

  libh::detail::backend::cuda::hash_by_key(keys.begin(), keys.end(), values.begin(), hash_table.begin(), hash_table.end(), hf, constrained_hash_access, max_age);

  // copy results back, if necessary
  // XXX this epilogue belongs somewhere else
  if(!thrust::detail::is_trivial_iterator<T_RAND_ACCESS_ITERATOR1>::value)
    thrust::copy(hash_table.begin(), hash_table.end(), hash_table_begin);
  
} // end hash_by_key()

} // end dispatch

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
  return libh::detail::backend::dispatch::hash(keys_begin, keys_end, hash_table_begin, hash_table_end, hf, constrained_hash_access, max_age,
    typename thrust::iterator_space<T_RAND_ACCESS_ITERATOR1>::type());
} // end hash()

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
  libh::detail::backend::dispatch::hash_by_key(keys_begin, keys_end, values_begin, hash_table_begin, hash_table_end, hf, constrained_hash_access, max_age,
  typename thrust::detail::minimum_space<
    typename thrust::iterator_space<T_RAND_ACCESS_ITERATOR1>::type,
    typename thrust::iterator_space<T_RAND_ACCESS_ITERATOR1>::type,
    typename thrust::iterator_space<T_RAND_ACCESS_ITERATOR2>::type,
    typename thrust::iterator_space<T_RAND_ACCESS_ITERATOR3>::type,
    typename thrust::iterator_space<T_RAND_ACCESS_ITERATOR3>::type
    >::type());

} // end hash_by_key()

} // end namespace backend
} // end namespace detail
} // end namespace libh

