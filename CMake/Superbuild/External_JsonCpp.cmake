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

set( proj JsonCpp )

# Sanity checks.
if( DEFINED ${proj}_DIR AND NOT EXISTS ${${proj}_DIR} )
  message( FATAL_ERROR
    "${proj}_DIR variable is defined as ${${proj}_DIR} which corresponds to a nonexistent directory" )
endif( DEFINED ${proj}_DIR AND NOT EXISTS ${${proj}_DIR} )

set( ${proj}_DEPENDENCIES "" )

# Include dependent projects, if any.
CheckExternalProjectDependency( ${proj} )

if( NOT DEFINED ${proj}_DIR AND NOT ${USE_SYSTEM_JSONCPP} )
  set( ${proj}_SOURCE_DIR ${CMAKE_BINARY_DIR}/${proj} )
  set( ${proj}_DIR ${CMAKE_BINARY_DIR}/${proj}-build )

  ExternalProject_Add( ${proj}
    URL ${${proj}_URL}
    URL_MD5 ${${proj}_HASH_OR_TAG}
    DOWNLOAD_DIR ${${proj}_SOURCE_DIR}
    SOURCE_DIR ${${proj}_SOURCE_DIR}
    BINARY_DIR ${${proj}_DIR}
    INSTALL_DIR ${${proj}_DIR}
    LOG_DOWNLOAD 1
    LOG_UPDATE 0
    LOG_CONFIGURE 0
    LOG_BUILD 0
    LOG_TEST 0
    LOG_INSTALL 0
    UPDATE_COMMAND ""
    CMAKE_GENERATOR ${gen}
    CMAKE_ARGS
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
      -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
      -DCMAKE_BUILD_TYPE:STRING=${build_type}
      ${CMAKE_OSX_EXTERNAL_PROJECT_ARGS}
      -DBUILD_SHARED_LIBS:BOOL=OFF
      -DJSONCPP_LIB_BUILD_SHARED:BOOL=OFF
      -DJSONCPP_WITH_TESTS:BOOL=OFF
    INSTALL_COMMAND "" )

else( NOT DEFINED ${proj}_DIR AND NOT ${USE_SYSTEM_JSONCPP} )
  if( ${USE_SYSTEM_JSONCPP} )
    find_package( ${proj} REQUIRED )
  endif( ${USE_SYSTEM_JSONCPP} )

  AddEmptyExternalProject( ${proj} "${${proj}_DEPENDENCIES}" )
endif( NOT DEFINED ${proj}_DIR AND NOT ${USE_SYSTEM_JSONCPP} )
