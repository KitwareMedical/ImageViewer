/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    vtkLookupTableEquivalencyHash.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "vtkLookupTableEquivalencyHash.h"

#include <iostream>

bool vtkLookupTableEquivalencyHash::Add(unsigned long a, unsigned long b)
{
  //
  // The order of the equivalence is important.
  //
  std::pair<Iterator, bool> result;
  if (a == b) return false;
  result = m_HashMap.insert( ValueType(a, b) );

  return result.second;

  //
  // The order of equivalence is unimportant.
  //
  //  unsigned long temp;
  //  std::pair<Iterator, bool> result;
  //  if (a == b) return false;
  //  else if (a < b)
  //    {  // swap a, b
  //      temp = a;
  //      a = b;
  //      b = temp;
  //    }
  //  result = m_HashMap.insert( ValueType(a, b) );
  //  
  //  if (result.second == false)
  //    { // Stop endless loops.
  //      if ( (*(result.first)).second  == b ) return false;
  //      else return (this->Add((*(result.first)).second, b));
  //    }
  //  else return true;
}
  
void vtkLookupTableEquivalencyHash::PrintHashTable()
{
  ConstIterator it = this->Begin();
  while (it != this->End() )
    {
      std::cout << (*it).first << " = " << (*it).second << std::endl;
      it++;
    }
}

void vtkLookupTableEquivalencyHash::Flatten()
{
  Iterator it = this->Begin();
  while ( it != this->End() )
    {
      (*it).second = this->RecursiveLookup((*it).first);
      it++;
    }
}

unsigned long vtkLookupTableEquivalencyHash::RecursiveLookup(const unsigned a) const
{
  unsigned long ans = a;
  unsigned long last_ans;
  while ( this->IsEntry(ans) )
    {
      ans = this->Lookup(ans);
      if (ans == a ) return last_ans; // about to cycle again.
      last_ans = ans;
    }
  return ans;

}
