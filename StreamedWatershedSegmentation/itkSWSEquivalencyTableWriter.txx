/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSEquivalencyTableWriter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#ifndef __itkSWSEquivalencyTableWriter_txx
#define __itkSWSEquivalencyTableWriter_txx

#include <iostream>
#include <fstream>

namespace itk
{
namespace sws
{

void EquivalencyTableWriter
::SafeWrite()
{
  const unsigned BUFSZ = 16384;
  unsigned long tablesz;

  EquivalencyTableType::Pointer input = this->GetInput();
  typedef std::pair< unsigned long, unsigned long> VType;
  
  std::ofstream out(m_FileName.c_str());
  if (!out)
    {
      throw ExceptionObject(__FILE__, __LINE__);
    }

  // Write the header, the number of records
  tablesz = input->Size();
  out.write((char *)&tablesz, sizeof(unsigned long));

 // now write data
  VType *buf = new VType[BUFSZ];
  
  EquivalencyTableType::Iterator it;

  it = input->Begin();
  unsigned n = 0;
  while ( it != input->End() )
    {
      (buf[n]).first  = (*it).first;
      (buf[n]).second = (*it).second;
      n++;
      ++it;
      if (n == BUFSZ)
        {
          out.write((char *)buf,   sizeof (VType) *  BUFSZ);
          n = 0;
        }
    }
  out.write((char *)buf,  sizeof ( VType) *  n);

  out.close();
  delete[] buf; 
}
  
} // end namespace sws
} // end namespace itk

#endif

