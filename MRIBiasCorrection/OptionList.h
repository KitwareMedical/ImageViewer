/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    OptionList.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __OPTIONLIST_H_
#define __OPTIONLIST_H_

#include <itkWin32Header.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "itkArray.h"

class OptionList
{
public:
  typedef std::multimap<std::string, std::string> OptionMap ;
  typedef std::vector<std::string> StringVector ;

  OptionList(int argc, char* argv[]) ;
  ~OptionList() {}

  class RequiredOptionMissing 
  {
  public:
    RequiredOptionMissing(std::string tag)
    {
      OptionTag = tag ;
    }

    std::string OptionTag ;
  } ;
 
  int GetOption(std::string option_tag, StringVector* values) ;
  int DumpOption(std::string option_tag, bool withTag = true, 
                 bool withNewLine = false) ;

  int GetMultiDoubleOption(std::string tag, 
                           std::vector<double>* args, 
                           bool required) ;

  int GetMultiDoubleOption(std::string tag, 
                           itk::Array<double>* args, 
                           bool required) ;


  double GetDoubleOption(std::string tag, double default_value, bool required);

  bool GetBooleanOption(std::string tag, bool default_value, bool required);

  int GetMultiIntOption(std::string tag, 
                        std::vector<int>* args,
                        bool required);

  int GetIntOption(std::string tag, int default_value, bool required) ;
  
  int GetStringOption(std::string tag, std::string* ret, bool required);
  
  int GetMultiStringOption(std::string tag, std::vector< std::string >* ret,
                           bool required);
protected:

private:
  OptionMap m_Map ;
} ; // end of class

#endif 
