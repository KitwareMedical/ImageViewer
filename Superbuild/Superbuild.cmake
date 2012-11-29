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

# Compute -G arg for configuring external projects with the same CMake gener    ator:
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
ExternalProject_Add( ImageViewer
  DEPENDS ${ImageViewer_DEPENDENCIES}
  DOWNLOAD_COMMAND ""
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
  BINARY_DIR ImageViewer-build
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    ${ep_common_args}
    -DBUILD_SHARED_LIBS:BOOL=FALSE
     # ITK
    -DITK_DIR:PATH=${ITK_DIR}
    # FLTK
    -DFLTK_DIR:PATH=${FLTK_DIR}
    ${_fltk_use_resource}
    -DImageViewer_USE_SUPERBUILD:BOOL=OFF
  INSTALL_COMMAND ""
)
