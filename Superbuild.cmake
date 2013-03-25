find_package(Git)
if(NOT GIT_FOUND)
  message(ERROR "Cannot find git. git is required for Superbuild")
endif()
find_package(Subversion)
if(NOT SUBVERSION_FOUND)
  message(ERROR "Cannot find subversion. subversion is required for Superbuild")
endif()

option( USE_GIT_PROTOCOL "If behind a firewall turn this off to use http instead." ON)

set(git_protocol "git")
if(NOT USE_GIT_PROTOCOL)
  set(git_protocol "http")
endif()

include( ExternalProject )

# Compute -G arg for configuring external projects with the same CMake generator:
if(CMAKE_EXTRA_GENERATOR)
  set(gen "${CMAKE_EXTRA_GENERATOR} - ${CMAKE_GENERATOR}")
else()
  set(gen "${CMAKE_GENERATOR}" )
endif()

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES:STRING=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=${CMAKE_OSX_DEPLOYMENT_TARGET}
    -DCMAKE_OSX_SYSROOT:PATH=${CMAKE_OSX_SYSROOT}
  )
endif(APPLE)

set(ep_common_args
  -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
  -DCMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}
  -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
  -DCMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}
  -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
  -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
  -DCMAKE_GENERATOR:STRING=${CMAKE_GENERATOR}
  -DCMAKE_EXTRA_GENERATOR:STRING=${CMAKE_EXTRA_GENERATOR}
  ${CMAKE_OSX_EXTERNAL_PROJECT_ARGS}
  )

set( ITKApps_DEPENDENCIES )

if(NOT VTK_DIR)
  include( ${CMAKE_SOURCE_DIR}/External-VTK.cmake )
  list( APPEND ITKApps_DEPENDENCIES VTK )
endif()

if(NOT ITK_DIR)
  include( ${CMAKE_SOURCE_DIR}/External-ITK.cmake )
  list( APPEND ITKApps_DEPENDENCIES ITK )
endif()

if(NOT FLTK_DIR)
  include( ${CMAKE_SOURCE_DIR}/External-FLTK.cmake )
  list( APPEND ITKApps_DEPENDENCIES FLTK )
endif()

set( _fltk_use_resource )
if(APPLE AND NOT FLTK_USE_X)
  set( _fltk_use_resource "-DITK_FLTK_RESOURCE:FILEPATH=Carbon.r" )
endif()
ExternalProject_Add( ITKApps
  DEPENDS ${ITKApps_DEPENDENCIES}
  DOWNLOAD_COMMAND ""
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..
  BINARY_DIR ITKApps-build
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    ${ep_common_args}
    -DBUILD_SHARED_LIBS:BOOL=FALSE
     # ITK
    -DITK_DIR:PATH=${ITK_DIR}
    # VTK
    -DUSE_VTK:BOOL=ON
    -DVTK_DIR:PATH=${VTK_DIR}
    # FLTK
    -DUSE_FLTK:BOOL=ON
    -DFLTK_DIR:PATH=${FLTK_DIR}
    ${_fltk_use_resource}
  INSTALL_COMMAND ""
)
