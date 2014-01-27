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

cmake_minimum_required( VERSION 2.8.7 )
project( ImageViewer )

option( USE_FLTK "Build FLTK, turn it False to use Qt instead." ON )

set( CMAKE_MODULE_PATH ${ImageViewer_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH} )#${CTK_CMAKE_DIR} $#{CTK_CMAKE_UTILITIES_DIR} )

option( ImageViewer_USE_SUPERBUILD
  "Build ImageViewer and projects it depends on using superbuild." ON )
mark_as_advanced( ImageViewer_USE_SUPERBUILD )

option( USE_SYSTEM_ITK "Exclude ITK from superbuild and use an existing build."
  OFF )
mark_as_advanced( USE_SYSTEM_ITK )

if( ImageViewer_USE_SUPERBUILD )
  add_subdirectory( ${ImageViewer_SOURCE_DIR}/CMake/Superbuild )
else( ImageViewer_USE_SUPERBUILD )

  find_package( ITK REQUIRED )
  include( ${ITK_USE_FILE} )

  if( USE_FLTK )

    option( USE_SYSTEM_FLTK "Exclude FLTK from superbuild and use an existing build."
     OFF )
    mark_as_advanced( USE_SYSTEM_FLTK )

    find_package( FLTK REQUIRED )

    if( DEFINED FLTK_USE_FILE AND EXISTS ${FLTK_USE_FILE} )
      set( FLTK_EXE_LINKER_FLAGS "" )
      include( ${FLTK_USE_FILE} )
      if( APPLE )
        list( APPEND FLTK_LIBRARIES "-framework Cocoa" )
      endif( APPLE )
    endif( DEFINED FLTK_USE_FILE AND  EXISTS ${FLTK_USE_FILE} )

    include( DisableFLTKWarnings )
    include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/FltkImageViewer )

    add_subdirectory( FltkImageViewer )
    add_subdirectory( ImageViewer )

  else( USE_FLTK )

    #option( USE_SYSTEM_CTK "Exclude CTK from superbuild and use an existing build."
     #OFF )
    #mark_as_advanced( USE_SYSTEM_CTK )
    
    find_package( Qt4 COMPONENTS QtCore QtGui QtNetwork QtOpenGL REQUIRED )
    include( ${QT_USE_FILE} )

    #set( VTK_DIR "/Users/kitware/Work/TubeTK-ImageViewer-CTKBuild/CTK-build/VTK-build" )
    #find_package( VTK REQUIRED )
    #include( ${VTK_USE_FILE} )


    #find_package( CTK REQUIRED )
    #include( ${CTK_USE_FILE} )

    include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Auxiliary ) #QtImageViewer

    add_subdirectory( Auxiliary ) #QtImageViewer

  endif( USE_FLTK )

  #add_subdirectory( ImageViewer )

endif( ImageViewer_USE_SUPERBUILD )
