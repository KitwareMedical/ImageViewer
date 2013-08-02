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
if( DEFINED ITK_DIR AND NOT EXISTS ${ITK_DIR} )
  message( FATAL_ERROR "ITK_DIR variable is defined but corresponds to a nonexistent directory" )
endif( DEFINED ITK_DIR AND NOT EXISTS ${ITK_DIR} )

set( ITK_DEPENDENCIES "" )

# Include dependent projects, if any.
CheckExternalProjectDependency( ITK )

if( NOT DEFINED ITK_DIR AND NOT ${USE_SYSTEM_ITK} )
  set( ITK_SOURCE_DIR ${CMAKE_BINARY_DIR}/ITK )
  set( ITK_DIR ${CMAKE_BINARY_DIR}/ITK-build )

  ExternalProject_Add( ITK
    GIT_REPOSITORY ${ITK_GIT_REPOSITORY}
    GIT_TAG ${ITK_GIT_TAG}
    DOWNLOAD_DIR ${ITK_SOURCE_DIR}
    SOURCE_DIR ${ITK_SOURCE_DIR}
    BINARY_DIR ${ITK_DIR}
    INSTALL_DIR ${ITK_DIR}
    CMAKE_GENERATOR ${gen}
    CMAKE_ARGS
      -DCMAKE_BUILD_TYPE:STRING=${build_type}
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
      -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
      -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
      -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
      ${CMAKE_OSX_EXTERNAL_PROJECT_ARGS}
      -DBUILD_EXAMPLES:BOOL=OFF
      -DBUILD_SHARED_LIBS:BOOL=${shared}
      -DBUILD_TESTING:BOOL=OFF
      -DITKV3_COMPATIBILITY:BOOL=ON
      -DITK_BUILD_ALL_MODULES:BOOL=ON
      -DITK_INSTALL_NO_DEVELOPMENT:BOOL=ON
      -DITK_LEGACY_REMOVE:BOOL=OFF
      -DITK_USE_REVIEW:BOOL=ON
    INSTALL_COMMAND "" )

else( NOT DEFINED ITK_DIR AND NOT ${USE_SYSTEM_ITK} )
  if( ${USE_SYSTEM_ITK} )
    find_package( ITK REQUIRED )
  endif( ${USE_SYSTEM_ITK} )

  AddEmptyExternalProject( ITK "${ITK_DEPENDENCIES}" )
endif( NOT DEFINED ITK_DIR AND NOT ${USE_SYSTEM_ITK} )
