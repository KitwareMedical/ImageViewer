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

set(MODULE_NAME ImageViewer)
set(${MODULE_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR})

list(APPEND ${MODULE_NAME}_INCLUDE_DIRS
    ${${MODULE_NAME}_SOURCE_DIR}/QtImageViewer
    ${${MODULE_NAME}_BINARY_DIR}/QtImageViewer)
include_directories(${${MODULE_NAME}_INCLUDE_DIRS})

set(${MODULE_NAME}_SRCS ImageViewer.cxx)
set(${MODULE_NAME}_LIBRARIES
    ${ITK_LIBRARIES}
    ${QT_LIBRARIES} QtImageViewer
    ${OPENGL_LIBRARIES})

set(BUILD_EXEC_ONLY EXECUTABLE_ONLY)
if(BUILD_SHARED_LIBS)
  set(BUILD_EXEC_ONLY)
endif(BUILD_SHARED_LIBS)

if(USE_CLI)

  SEMMacroBuildCLI(
    ${BUILD_EXEC_ONLY}
    NAME ${MODULE_NAME}
    ADDITIONAL_SRCS
      ${${MODULE_NAME}_SRCS}
      ${${MODULE_NAME}_GUI_SRCS}
      ${${MODULE_NAME}_QRC_SRCS}
      ${${MODULE_NAME}_UI_CXX}
    TARGET_LIBRARIES
      ${${MODULE_NAME}_LIBRARIES}
    INCLUDE_DIRECTORIES
      ${${MODULE_NAME}_INCLUDE_DIRS})
else(USE_CLI)
  add_executable(${MODULE_NAME} ${${MODULE_NAME}_SRCS} ${${MODULE_NAME}_UI_CXX})

  set_target_properties(${MODULE_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )

  target_link_libraries(${MODULE_NAME}
    ${${MODULE_NAME}_LIBRARIES}
    )
endif(USE_CLI)
