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

# Sanity checks.
if( DEFINED SlicerExecutionModel_DIR AND NOT EXISTS ${SlicerExecutionModel_DIR} )
  message( FATAL_ERROR "SlicerExecutionModel_DIR variable is defined but corresponds to a nonexistent directory" )
endif( DEFINED SlicerExecutionModel_DIR AND NOT EXISTS ${SlicerExecutionModel_DIR} )

# Set dependency list
set( SlicerExecutionModel_DEPENDENCIES "ITK" )

# Include dependent projects, if any.
CheckExternalProjectDependency( SlicerExecutionModel )

if( NOT DEFINED SlicerExecutionModel_DIR AND NOT USE_SYSTEM_SlicerExecutionModel )
  set( SlicerExecutionModel_SOURCE_DIR ${CMAKE_BINARY_DIR}/SlicerExecutionModel )
  set( SlicerExecutionModel_DIR ${CMAKE_BINARY_DIR}/SlicerExecutionModel-build )

  ExternalProject_Add( SlicerExecutionModel
    GIT_REPOSITORY ${SlicerExecutionModel_URL}
    GIT_TAG ${SlicerExecutionModel_HASH_OR_TAG}
    DOWNLOAD_DIR ${SlicerExecutionModel_SOURCE_DIR}
    SOURCE_DIR ${SlicerExecutionModel_SOURCE_DIR}
    BINARY_DIR ${SlicerExecutionModel_DIR}
    INSTALL_DIR ${SlicerExecutionModel_DIR}
    CMAKE_GENERATOR ${gen}
    LOG_DOWNLOAD 1
    LOG_UPDATE 0
    LOG_CONFIGURE 0
    LOG_BUILD 0
    LOG_TEST 0
    LOG_INSTALL 0
    UPDATE_COMMAND ""
    CMAKE_ARGS
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
      -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
      -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
      -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
      -DCMAKE_BUILD_TYPE:STRING=${build_type}
      ${CMAKE_OSX_EXTERNAL_PROJECT_ARGS}
      -DBUILD_SHARED_LIBS:BOOL=${shared}
      -DBUILD_TESTING:BOOL=OFF
      -DITK_DIR:PATH=${ITK_DIR}
    INSTALL_COMMAND ""
    DEPENDS
      ${SlicerExecutionModel_DEPENDENCIES} )
else( NOT DEFINED SlicerExecutionModel_DIR AND NOT USE_SYSTEM_SlicerExecutionModel )
  if( USE_SYSTEM_SlicerExecutionModel )
    find_package( SlicerExecutionModel REQUIRED )
  endif( USE_SYSTEM_SlicerExecutionModel )

  AddEmptyExternalProject( SlicerExecutionModel "${SlicerExecutionModel_DEPENDENCIES}" )
endif( NOT DEFINED SlicerExecutionModel_DIR AND NOT USE_SYSTEM_SlicerExecutionModel )

