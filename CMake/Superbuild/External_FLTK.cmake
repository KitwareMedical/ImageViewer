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

# Make sure this file is included only once.
get_filename_component( CMAKE_CURRENT_LIST_FILENAME ${CMAKE_CURRENT_LIST_FILE}
  NAME_WE )
if( ${CMAKE_CURRENT_LIST_FILENAME}_FILE_INCLUDED )
  return()
endif( ${CMAKE_CURRENT_LIST_FILENAME}_FILE_INCLUDED )
set( ${CMAKE_CURRENT_LIST_FILENAME}_FILE_INCLUDED 1 )

set( FLTK_DEPENDENCIES "" )

# Include dependent projects, if any.
CheckExternalProjectDependency( FLTK )

if( NOT DEFINED FLTK_DIR AND NOT ${USE_SYSTEM_FLTK} )
  set( FLTK_SOURCE_DIR ${CMAKE_BINARY_DIR}/FLTK )
  set( FLTK_DIR ${CMAKE_BINARY_DIR}/FLTK-build )

  ExternalProject_Add( FLTK
    URL ${FLTK_URL}
    URL_MD5 ${FLTK_URL_MD5_TAG}
    DOWNLOAD_DIR ${FLTK_SOURCE_DIR}
    SOURCE_DIR ${FLTK_SOURCE_DIR}
    BINARY_DIR ${FLTK_DIR}
    INSTALL_DIR ${FLTK_DIR}
    CMAKE_GENERATOR ${gen}
    LOG_DOWNLOAD 1
    LOG_UPDATE 0
    LOG_CONFIGURE 0
    LOG_BUILD 0
    LOG_TEST 0
    LOG_INSTALL 0
    CMAKE_ARGS
      -DCMAKE_BUILD_TYPE:STRING=${build_type}
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
      -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
      -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
      -DCMAKE_INSTALL_PREFIX:PATH=${FLTK_DIR}
      ${CMAKE_OSX_EXTERNAL_PROJECT_ARGS}
      -DOPTION_BUILD_EXAMPLES:BOOL=OFF
      -DOPTION_PREFIX_CONFIG=${FLTK_DIR} )
else( NOT DEFINED FLTK_DIR AND NOT ${USE_SYSTEM_FLTK} )
  if( ${USE_SYSTEM_FLTK} )
    find_package( FLTK REQUIRED )
  endif( ${USE_SYSTEM_FLTK} )

  AddEmptyExternalProject( FLTK "${FLTK_DEPENDENCIES}" )
endif( NOT DEFINED FLTK_DIR AND NOT ${USE_SYSTEM_FLTK} )
