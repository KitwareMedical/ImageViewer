/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSWSSegmentTreeReader.txx
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
#ifndef __itkSWSSegmentTreeReader_txx
#define __itkSWSSegmentTreeReader_txx

#include <iostream>
#include <fstream>

namespace itk
{
namespace sws
{

template <class TScalarType>
void SegmentTreeReader<TScalarType>
::GenerateData()
{
  const unsigned BUFSZ = 16384;

  typename SegmentTreeType::Pointer output = this->GetOutput();

  std::ifstream in(m_FileName.c_str());
  if (!in)
    {
      throw ExceptionObject(__FILE__, __LINE__);
    }

  // read header
  unsigned long listsz;
  in.read((char *)&listsz, sizeof(unsigned long));

  // now read data
  typename SegmentTreeType::ValueType *buf =
    new typename SegmentTreeType::ValueType[BUFSZ];
  typename SegmentTreeType::Iterator it;
  unsigned segsize = sizeof( typename SegmentTreeType::ValueType );
  unsigned n = 0;
  while ( ! in.eof() )
    {

      in.read((char *)buf, segsize * BUFSZ );
      n = in.gcount();

      for (unsigned int i = 0; i < (n / segsize );  ++i) 
        {          output->PushBack(buf[i]);  }
    }

  in.close();
  delete[] buf; 
}

} // end namespace sws
} // end namespace itk

#endif

