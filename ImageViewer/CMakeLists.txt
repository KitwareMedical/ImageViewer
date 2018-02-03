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

set( MODULE_NAME ImageViewer )
set( ${MODULE_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR} )

list( APPEND ${MODULE_NAME}_INCLUDE_DIRS
  ${${MODULE_NAME}_SOURCE_DIR}/QtImageViewer
  ${${MODULE_NAME}_BINARY_DIR}/QtImageViewer
  )
include_directories( ${${MODULE_NAME}_INCLUDE_DIRS} )

set( ${MODULE_NAME}_SRCS ImageViewer.cxx )
set( ${MODULE_NAME}_LIBRARIES
  ${ITK_LIBRARIES}
  QtImageViewer
  Qt5::Core
  Qt5::Gui
  Qt5::OpenGL
  Qt5::Widgets
  ${OPENGL_LIBRARIES}
  )

SEMMacroBuildCLI(
  NAME ${MODULE_NAME}
  ADDITIONAL_SRCS
    ${${MODULE_NAME}_SRCS}
    ${${MODULE_NAME}_GUI_SRCS}
    ${${MODULE_NAME}_QRC_SRCS}
    ${${MODULE_NAME}_UI_CXX}
  TARGET_LIBRARIES
    ${${MODULE_NAME}_LIBRARIES}
  INCLUDE_DIRECTORIES
    ${${MODULE_NAME}_INCLUDE_DIRS}
  )
