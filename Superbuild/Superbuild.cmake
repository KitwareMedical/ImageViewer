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

# Compute -G arg for configuring external projects with the same CMake generator.
if(CMAKE_EXTRA_GENERATOR)
  set(gen "${CMAKE_EXTRA_GENERATOR} - ${CMAKE_GENERATOR}")
else()
  set(gen "${CMAKE_GENERATOR}" )
endif()

set( ImageViewer_DEPENDENCIES )

if(NOT ITK_DIR)
  include( ${CMAKE_CURRENT_SOURCE_DIR}/Superbuild/External-ITK.cmake )
  list( APPEND ImageViewer_DEPENDENCIES ITK )
endif()

if(NOT FLTK_DIR)
  include( ${CMAKE_CURRENT_SOURCE_DIR}/Superbuild/External-FLTK.cmake )
  list( APPEND ImageViewer_DEPENDENCIES FLTK )
endif()

set( _fltk_use_resource )
if(APPLE AND NOT FLTK_USE_X)
  set( _fltk_use_resource "-DITK_FLTK_RESOURCE:FILEPATH=Carbon.r" )
endif()


# Use for CMAKE_OSX_* in external projects.
set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES:STRING=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=${CMAKE_OSX_DEPLOYMENT_TARGET}
    -DCMAKE_OSX_SYSROOT:PATH=${CMAKE_OSX_SYSROOT}
  )
endif(APPLE)

ExternalProject_Add( ImageViewer
  DEPENDS ${ImageViewer_DEPENDENCIES}
  DOWNLOAD_COMMAND ""
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
  BINARY_DIR ImageViewer-build
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    ${ep_common_args}
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
    -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
    ${CMAKE_OSX_EXTERNAL_PROJECT_ARGS}
    -DBUILD_SHARED_LIBS:BOOL=FALSE
     # ITK
    -DITK_DIR:PATH=${ITK_DIR}
    # FLTK
    -DFLTK_DIR:PATH=${FLTK_DIR}
    ${_fltk_use_resource}
    -DImageViewer_USE_SUPERBUILD:BOOL=OFF
  INSTALL_COMMAND ""
)
