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

if(USE_ITK_FILE)
   include(${USE_ITK_FILE})
endif(USE_ITK_FILE)

include_directories(${QT_INCLUDE_PATH}
  ${QT_INCLUDE_DIR}
  ${ImageViewer_BINARY_DIR}/QtImageViewer
  ${ImageViewer_SOURCE_DIR}/QtImageViewer
  )

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})#${GLUT_INCLUDE_DIRS}

set(QtSlicer_SRCS
  QtGlSliceView.cxx
  QtSlicer.cxx
  )

set(QtSlicer_GUI_SRCS
  Resources/UI/QtSlicerGUI.ui
  Resources/UI/QtSlicerHelpGUI.ui
  )

set(QtSlicer_MOC_SRCS
  QtGlSliceView.h
  QtSlicer.h
  )

set(QtSlicer_RESOURCES
  Resources/qtImageViewerResources.qrc
  )

QT4_WRAP_UI(QtSlicer_UI_CXX ${QtSlicer_GUI_SRCS})
QT4_WRAP_CPP(QtSlicer_SRCS ${QtSlicer_MOC_SRCS})
QT4_ADD_RESOURCES(QtSlicer_QRC_SRCS ${QtSlicer_RESOURCES})

add_definitions(-DQT_DLL)
add_definitions(-DQT_THREAD_SUPPORT)

add_library(QtImageViewer STATIC ${QtSlicer_SRCS}
  ${QtSlicer_GUI_SRCS}
  ${QtSlicer_UI_CXX}
  ${QtSlicer_QRC_SRCS}
  )

install(TARGETS QtImageViewer DESTINATION lib)
target_link_libraries(QtImageViewer ${QT_LIBRARIES})
