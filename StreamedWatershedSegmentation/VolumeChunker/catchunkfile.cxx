/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    catchunkfile.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <fstream>
#include <iostream>
#include "chunks.h"

/*
 * This application prints chunked volume records (external header files).
 */
int main(int argc, char *argv[])
{
  int chunknumber;
  ctk::chunk_info_struct *chunk_list;
  if (argc < 2)
    {
      std::cerr << "Usage: catchunkfile <filename>" << std::endl;
      ::exit(1);
    }
  
  std::ifstream in;
  in.open(argv[1]);
  if (!in)
    {
      std::cerr << "Could not open file" << std::endl;
      ::exit(1);
    }
  
  in.read((char *)&chunknumber, sizeof(int));
  chunk_list = new ctk::chunk_info_struct[chunknumber];
  in.read((char *)chunk_list, chunknumber *
          sizeof(ctk::chunk_info_struct));
  in.close();

  // List the chunks
  std::cout << chunknumber << " records" << std::endl;
  for (int i = 0; i < chunknumber; ++i)
    {
      std::cout << chunk_list[i] << std::endl;
    }

  delete[] chunk_list;
  
  return 0;
}


