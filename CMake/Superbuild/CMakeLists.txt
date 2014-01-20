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

set( CMAKE_MODULE_PATH ${ImageViewer_SOURCE_DIR}/CMake/Superbuild
  ${CMAKE_MODULE_PATH} )

include( ExternalProject )
include( AddEmptyExternalProject )
include( CheckExternalProjectDependency )

set( base ${CMAKE_BINARY_DIR} )
set_property( DIRECTORY PROPERTY EP_BASE ${base} )

set( shared ON )
if( DEFINED BUILD_SHARED_LIBS )
  set( shared ${BUILD_SHARED_LIBS} )
endif( DEFINED BUILD_SHARED_LIBS )

set( build_type "Debug" )
if( CMAKE_BUILD_TYPE )
  set( build_type "${CMAKE_BUILD_TYPE}" )
endif( CMAKE_BUILD_TYPE )

set( gen "${CMAKE_GENERATOR}" )

include( ExternalProjectsConfig )
include( SetOSXVariables )

set( ImageViewer_DEPENDENCIES "ITK" )
if( USE_FLTK )
  list( APPEND ImageViewer_DEPENDENCIES "FLTK" )
else( USE_FLTK )
  list( APPEND ImageViewer_DEPENDENCIES "CTK" )
endif( USE_FLTK )

set( EXTERNAL_PROJECT_DIR ${ImageViewer_SOURCE_DIR}/CMake/Superbuild )
CheckExternalProjectDependency( ImageViewer )

set( ImageViewer_DIR ${CMAKE_BINARY_DIR}/ImageViewer-build )

ExternalProject_Add( ImageViewer
  DOWNLOAD_COMMAND ""
  DOWNLOAD_DIR ${ImageViewer_SOURCE_DIR}
  SOURCE_DIR ${ImageViewer_SOURCE_DIR}
  BINARY_DIR ${ImageViewer_DIR}
  INSTALL_DIR ${ImageViewer_DIR}
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${build_type}
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
    -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
    ${CMAKE_OSX_EXTERNAL_PROJECT_ARGS}
    -DBUILD_SHARED_LIBS:BOOL=OFF
    -DFLTK_DIR:PATH=${FLTK_DIR}
    -DITK_DIR:PATH=${ITK_DIR}
    #-DQT_QMAKE_EXECUTABLE:PATH=${QT_QMAKE_EXECUTABLE}
    -DCTK_DIR:PATH=${CTK_DIR}
    -DVTK_DIR:PATH=${VTK_DIR}
    -DImageViewer_USE_SUPERBUILD:BOOL=OFF
    -DUSE_FLTK:BOOL=${USE_FLTK}
  INSTALL_COMMAND ""
  DEPENDS ${ImageViewer_DEPENDENCIES} )
