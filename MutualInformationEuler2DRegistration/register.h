#include "itkFixedArray.h"
#include "itkArray.h"

template< class TSource, class TTarget >
void ConvertVectorToFixedArray(TSource &vec, TTarget& array)
{
  typename TSource::iterator v_iter = vec.begin() ;
  typename TTarget::Iterator a_iter = array.Begin() ;
  while (v_iter != vec.end())
    {
      *a_iter = (typename TTarget::ValueType) *v_iter ;
      std::cout << "    " <<*a_iter << std::endl ;
      ++v_iter ;
      ++a_iter ;
    }
} 

template< class TSource, class TTarget >
void ConvertVectorToArray(TSource &vec, TTarget& array)
{
  typename TSource::iterator v_iter = vec.begin() ;
  typename TTarget::iterator a_iter = array.begin() ;
  while (v_iter != vec.end())
    {
      *a_iter = (typename TTarget::ValueType) *v_iter ;
      std::cout << "    " <<*a_iter << std::endl ;
      ++v_iter ;
      ++a_iter ;
    }
} 
