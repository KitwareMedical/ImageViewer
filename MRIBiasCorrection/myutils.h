#ifndef __MYUTILS_H
#define __MYUTILS_H

#include <vnl/vnl_vector.h>

void print_line(std::string msg)
{
  std::cout << msg << std::endl ;
}

void dump(vnl_vector<double> vect)
{
  std::cout << "vector dump" << std::endl ;
  for (int i = 0 ; i < vect.size() ; i++)
    {
      std::cout << i << ": " << vect[i] << std::endl ;
    }
}
#endif
