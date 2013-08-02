##############################################################################
#
# Library:   TubeTK
#
# Copyright 2010 Kitware Inc. 28 Corporate Drive,
# Clifton Park, NY, 12065, USA.
#
# All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
##############################################################################

##############################################################################
#
# Program:   Insight Segmentation & Registration Toolkit
#
# Copyright (c) 2002 Insight Consortium. All rights reserved.
# See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.
#
#    This software is distributed WITHOUT ANY WARRANTY; without even
#    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#    PURPOSE.  See the above copyright notices for more information.
#
##############################################################################

# The Fluid-generated FLTK sources have many warnings. This macro will disable
# warnings for the generated files on some compilers.

macro( DisableFLTKWarnings files )
  if( CMAKE_COMPILER_IS_GNUCXX )
    foreach( f ${files} )
      string( REGEX REPLACE "\\.fl$" ".cxx" SRC "${f}" )
      string( REGEX REPLACE ".*/([^/]*)$" "\\1" SRC "${SRC}" )
      set_source_files_properties( ${SRC} PROPERTIES COMPILE_FLAGS -w )
    endforeach( f )
  endif( CMAKE_COMPILER_IS_GNUCXX )

  if( MSVC )
    foreach( f ${files} )
      string( REGEX REPLACE "\\.fl$" ".cxx" SRC "${f}" )
      string( REGEX REPLACE ".*/([^/]*)$" "\\1" SRC "${SRC}" )
      set_source_files_properties( ${SRC} PROPERTIES COMPILE_FLAGS /w )
    endforeach( f )
  endif( MSVC )
endmacro( DisableFLTKWarnings )
