##############################################################################
#
# Library:   TubeTK
#
# Copyright Kitware Inc.
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

cmake_minimum_required(VERSION 3.10)

project(ImageViewer)

cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0006 OLD)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_MODULE_PATH ${ImageViewer_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})

set_property(GLOBAL PROPERTY ImageViewer_LIBRARIES)
set_property(GLOBAL PROPERTY ImageViewer_INCLUDE_DIRS)
set_property(GLOBAL PROPERTY ImageViewer_LIBRARY_DIRS)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions( -DQT_THREAD_SUPPORT )
find_package(Qt5 5.11 COMPONENTS Core Widgets Gui REQUIRED)

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
