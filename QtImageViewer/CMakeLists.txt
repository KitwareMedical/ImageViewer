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
project( QtImageViewer )

include( GenerateExportHeader )

include_directories(
  ${ImageViewer_BINARY_DIR}/QtImageViewer
  ${ImageViewer_SOURCE_DIR}/QtImageViewer
  )

find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIRS} )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

set( QtImageViewer_SRCS
  QtGlSliceView.cxx
  QtImageViewer.cxx
  RulerWidget.cxx
  BoxWidget.cxx
  )

set( QtImageViewer_GUI_SRCS
  QtImageViewer.ui
  )

set( QtImageViewer_MOC_SRCS
  QtGlSliceView.h
  QtImageViewer.h
  RulerWidget.h
  BoxWidget.h
  )

add_library( QtImageViewer STATIC ${QtImageViewer_SRCS}
  ${QtImageViewer_GUI_SRCS}
  ${QtImageViewer_UI_CXX}
  ${QtImageViewer_QRC_SRCS}
  )

install( TARGETS QtImageViewer DESTINATION lib )

target_link_libraries( QtImageViewer PUBLIC
  ${ITK_LIBRARIES}
  Qt5::Widgets
  ${OPENGL_LIBRARIES}
  )

generate_export_header( QtImageViewer
  BASE_NAME QtImageViewer
  EXPORT_MACRO_NAME QtImageViewer_EXPORT
  EXPORT_FILE_NAME QtImageViewer_Export.h 
  )
