/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkLookupTableEquivalencyHash.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __vtkLookupTableEquivalencyHash_h_
#define __vtkLookupTableEquivalencyHash_h_
#include "itkWin32Header.h"
#include "itk_hash_map.h"
#include <list>

struct merge_t
{
  unsigned long from;
  unsigned long to;
  float saliency;
};


typedef std::list<merge_t> merge_list_t;

typedef std::list<unsigned long> unsigned_long_list_t;
class ITK_EXPORT vtkLookupTableEquivalencyHash
{
public:

  typedef vtkLookupTableEquivalencyHash Self;

  typedef itk::hash_map<unsigned long, unsigned long,
    itk::hash<unsigned long> > HashTableType;
  typedef HashTableType::iterator Iterator;
  typedef HashTableType::const_iterator ConstIterator;
  typedef HashTableType::value_type ValueType;
  /**
   * ``Flattens'' the equivalency table by eliminating all redundant and
   * recursive equivalencies.  I.e. the set { 2=1; 3=2; 4=3 } is converted
   * to {4=1; 3=1; 2=1}.
   */
  void Flatten();

  bool Add(unsigned long a, unsigned long b);
  
  unsigned long Lookup(const unsigned long a) const
    {
      ConstIterator result = m_HashMap.find(a);
      if ( result == m_HashMap.end() ) return a;
      else return (*result).second;
    }
  
  unsigned long RecursiveLookup(const unsigned a) const;
  
  bool IsEntry(const unsigned long a) const
    {
      if ( m_HashMap.find(a) == m_HashMap.end() ) return false;
      else return true;
    }

  void Erase(const unsigned long a)
    {  m_HashMap.erase(a); }
  
  void Clear()
    {      m_HashMap.clear();    }

  bool Empty() const
    {      return m_HashMap.empty();    }

  HashTableType::size_type Size() const
    {    return m_HashMap.size(); }
  
  Iterator Begin() { return m_HashMap.begin(); }
  Iterator End()   { return m_HashMap.end();   }

  void operator=(const vtkLookupTableEquivalencyHash &o)
    { this->m_HashMap = o.m_HashMap; }
  
  /**
   * Convenience methodx for debugging.
   */
  void PrintHashTable();

  
protected:
  HashTableType m_HashMap;
};


#endif
