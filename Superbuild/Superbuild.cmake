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
  include( ${CMAKE_SOURCE_DIR}/External-ITK.cmake )
  list( APPEND ImageViewer_DEPENDENCIES ITK )
endif()

if(NOT FLTK_DIR)
  include( ${CMAKE_SOURCE_DIR}/External-FLTK.cmake )
  list( APPEND ImageViewer_DEPENDENCIES FLTK )
endif()

ExternalProject_Add( ImageViewer
  DEPENDS ${ImageViewer_DEPENDENCIES}
  DOWNLOAD_COMMAND ""
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..
  BINARY_DIR ImageViewer-build
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    ${ep_common_args}
    -DBUILD_SHARED_LIBS:BOOL=FALSE
     # ITK
    -DITK_DIR:PATH=${ITK_DIR}
    # FLTK
    -DUSE_FLTK:BOOL=ON
    -DFLTK_DIR:PATH=${FLTK_DIR}
    -DITK_FLTK_RESOURCE:FILEPATH=Carbon.r
  INSTALL_COMMAND ""
)
