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

include( GenerateExportHeader )

include_directories(
  ${QT_INCLUDE_DIR}
  ${ImageViewer_BINARY_DIR}/QtImageViewer
  ${ImageViewer_SOURCE_DIR}/QtImageViewer
  )

find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIRS} )

set( QtImageViewer_SRCS
  QtGlSliceView.cxx
  QtImageViewer.cxx
  )

set( QtImageViewer_GUI_SRCS
  Resources/UI/QtImageViewer.ui
  )

set( QtImageViewer_MOC_SRCS
  QtGlSliceView.h
  QtImageViewer.h
  )

QT5_WRAP_UI( QtImageViewer_UI_CXX ${QtImageViewer_GUI_SRCS} )
QT5_WRAP_CPP( QtImageViewer_SRCS ${QtImageViewer_MOC_SRCS} )

add_definitions( -DQT_DLL )
add_definitions( -DQT_THREAD_SUPPORT )

add_library( QtImageViewer ${QtImageViewer_SRCS}
  ${QtImageViewer_GUI_SRCS}
  ${QtImageViewer_UI_CXX}
  ${QtImageViewer_QRC_SRCS}
  )

install( TARGETS QtImageViewer DESTINATION lib )
target_link_libraries( QtImageViewer
  ${ITK_LIBRARIES}
  Qt5::Core
  Qt5::Gui
  Qt5::OpenGL
  Qt5::Widgets
  ${OPENGL_LIBRARIES}
  )

cmake_policy( SET CMP0026 OLD )
get_target_property( location QtImageViewer LOCATION )
get_filename_component( output_directory ${location} PATH )

set_property( GLOBAL APPEND PROPERTY ImageViewer_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

set_property( GLOBAL APPEND PROPERTY ImageViewer_LIBRARIES QtImageViewer )
set_property( GLOBAL APPEND PROPERTY ImageViewer_LIBRARY_DIRS
  ${output_directory} )

generate_export_header( QtImageViewer
  BASE_NAME QtImageViewer
  EXPORT_MACRO_NAME QtImageViewer_EXPORT
  EXPORT_FILE_NAME QtImageViewer_Export.h 
  )
