/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    OptionList.cxx
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
#include "OptionList.h"

OptionList::OptionList(int argc, char* argv[])
{
  std::string tag ;
  std::string value ;
  
  int arg_type = 1 ;
  int prev_arg_type = 0 ;

  int index = 1 ;
  while (index < argc)
    {
      if (argv[index][0] == '-' && argv[index][1] == '-')
        {
          // tag
          tag = argv[index] ;
          tag = tag.erase(0, 2) ; // remove '--' 
        }            
      else
        {
          // value 
          value = argv[index] ;
          m_Map.insert(std::make_pair(tag, value)) ;
        }
      index++ ;
    }
}

int OptionList::GetOption(std::string option_tag, StringVector* values)
{
  values->clear() ;
  typedef OptionMap::const_iterator CI ;
  std::pair<CI, CI> bound = m_Map.equal_range(option_tag) ;
  int count = 0 ;
  
  for (CI i = bound.first ; i != bound.second ; ++i)
    {
      values->push_back(i->second) ;
      count++ ;
    }
  return count ;
}


int OptionList::DumpOption(std::string option_tag, bool withTag,
                           bool withNewLine)
{
  typedef OptionMap::const_iterator CI ;
  std::pair<CI, CI> bound = m_Map.equal_range(option_tag) ;
  int count = 0 ;

  if (bound.first != bound.second)
    {
      if (withTag)
        std::cout << "--" << option_tag << " " ;
      
      for (CI i = bound.first ; i != bound.second ; ++i)
        {
          std::cout << i->second << " " ;
          count++ ;
        }
      
      if (withNewLine)
        std::cout << std::endl ;
      
      return count++ ;
    }

    return 0 ;
}


int OptionList::GetMultiDoubleOption(std::string tag, 
                                     std::vector<double>* args, 
                                     bool required)
  throw (RequiredOptionMissing) 
{
  args->clear() ;
  
  StringVector temp_args ;
  int arg_no = this->GetOption(tag, &temp_args) ;
  
  if (required && arg_no == 0)
    throw RequiredOptionMissing(tag) ;
  
  if (arg_no == 0)
    return -1 ;
  
  if (temp_args[0] == "-")
    return -2 ;
  
  for (int i = 0 ; i < arg_no ; i++)
    {
      args->push_back( atof(temp_args[i].c_str()) ) ;
    }
  
  return arg_no ;
}

double OptionList::GetDoubleOption(std::string tag, bool required)
  throw (RequiredOptionMissing) 
{
  StringVector temp_args ;
  int arg_no = this->GetOption(tag, &temp_args) ;
  
  if (required && arg_no == 0)
    throw RequiredOptionMissing(tag) ;
  
  if (arg_no == 0)
    return -1 ;
  
  return atof(temp_args[0].c_str()) ;
}

bool OptionList::GetBooleanOption(std::string tag, bool required)
  throw (RequiredOptionMissing) 
{
  StringVector args ;
  int arg_no = this->GetOption(tag, &args) ;
  
  if (required && arg_no == 0)
    throw RequiredOptionMissing(tag) ;
  
  if (arg_no == 0)
    return false ;
  
  if (args[0] == "yes")
    {
      return true ;
    }
  else
    {
      return false ;
    }
}

int OptionList::GetMultiIntOption(std::string tag, 
                                  std::vector<int>* args, 
                                  bool required)
  throw (RequiredOptionMissing) 
{
  args->clear() ;
  
  StringVector temp_args ;
  int arg_no = this->GetOption(tag, &temp_args) ;
  
  if (required && arg_no == 0)
    throw RequiredOptionMissing(tag) ;
  
  if (arg_no == 0)
    return -1 ;
  
  if (temp_args[0] == "-")
    return -2 ;
  
  for (int i = 0 ; i < arg_no ; i++)
    {
      args->push_back( atoi(temp_args[i].c_str()) ) ;
    }
  
  return arg_no ;
}

int OptionList::GetIntOption(std::string tag, bool required)
  throw (RequiredOptionMissing) 
{
  StringVector args ;
  int arg_no = this->GetOption(tag, &args) ;
  
  if (required && arg_no == 0)
    throw RequiredOptionMissing(tag) ;
  
  if (arg_no == 0)
    return -1 ;
  
  return atoi(args[0].c_str()) ;
}

int OptionList::GetStringOption(std::string tag, 
                                std::string* ret, 
                                bool required)
  throw (RequiredOptionMissing) 
{
  StringVector args ;
  int arg_no = this->GetOption(tag, &args) ;
  
  if (required && arg_no == 0)
    throw RequiredOptionMissing(tag) ;
  
  if (arg_no == 0)
    return -1 ;
  
  *ret = args[0] ;
  return arg_no ;
}

