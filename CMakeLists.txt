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

cmake_minimum_required(VERSION 2.8.7)

project(ImageViewer)

set(CMAKE_MODULE_PATH ${ImageViewer_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})

option(ImageViewer_USE_SUPERBUILD
  "Build ImageViewer and projects it depends on using superbuild." ON)
mark_as_advanced(ImageViewer_USE_SUPERBUILD)

option(USE_SYSTEM_ITK 
  "Exclude ITK from superbuild and use an existing build." OFF)
mark_as_advanced(USE_SYSTEM_ITK)

option(USE_SYSTEM_SlicerExecutionModel 
  "Exclude SlicerExecutionModel from superbuild and use an existing build." OFF)
mark_as_advanced(USE_SYSTEM_SlicerExecutionModel)

set_property(GLOBAL PROPERTY ImageViewer_LIBRARIES)
set_property(GLOBAL PROPERTY ImageViewer_INCLUDE_DIRS)
set_property(GLOBAL PROPERTY ImageViewer_LIBRARY_DIRS)

if(UNIX)
  if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC"
         CACHE STRING "Flags used by all build types." FORCE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC"
         CACHE STRING "Flags used by all build types." FORCE)
  endif(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
endif(UNIX)

if(ImageViewer_USE_SUPERBUILD)

  add_subdirectory(CMake/Superbuild)
  return()

endif(ImageViewer_USE_SUPERBUILD)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(Qt5 COMPONENTS Core Widgets Gui OpenGL REQUIRED)

find_package(SlicerExecutionModel REQUIRED)
include(${SlicerExecutionModel_USE_FILE})

add_subdirectory(QtImageViewer)
add_subdirectory(ImageViewer)

get_property(ImageViewer_LIBRARIES GLOBAL PROPERTY ImageViewer_LIBRARIES)
get_property(ImageViewer_INCLUDE_DIRS GLOBAL PROPERTY ImageViewer_INCLUDE_DIRS)
get_property(ImageViewer_LIBRARY_DIRS GLOBAL PROPERTY ImageViewer_LIBRARY_DIRS)

set(ImageViewer_CONFIG_BINARY_DIR ${CMAKE_BINARY_DIR})
set(ImageViewer_USE_FILE
  ${ImageViewer_CONFIG_BINARY_DIR}/UseImageViewer.cmake )

configure_file(${ImageViewer_SOURCE_DIR}/CMake/ImageViewerConfig.cmake.in
  ${ImageViewer_CONFIG_BINARY_DIR}/ImageViewerConfig.cmake @ONLY)

# Configure UseTubeTK.cmake.
configure_file(${ImageViewer_SOURCE_DIR}/CMake/UseImageViewer.cmake.in
  ${ImageViewer_CONFIG_BINARY_DIR}/UseImageViewer.cmake @ONLY)

configure_file(${ImageViewer_SOURCE_DIR}/CMake/ImageViewerConfigure.h.in
  ${ImageViewer_CONFIG_BINARY_DIR}/ImageViewer/ImageViewerConfigure.h @ONLY)
