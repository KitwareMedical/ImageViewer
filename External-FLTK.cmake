#-----------------------------------------------------------------------------
# Get and build fltk
#
#-----------------------------------------------------------------------------
if( NOT WIN32)
  configure_file(
    ${CMAKE_SOURCE_DIR}/patch-FLTK.sh.in
    ${CMAKE_BINARY_DIR}/patch-FLTK.sh
    @only
  )
  set(FLTK_PATCH_COMMAND "${CMAKE_BINARY_DIR}/patch-FLTK.sh")
else ()
  set(FLTK_PATCH_COMMAND "")
endif()

set(FLTK_INSTALL_COMMAND ${CMAKE_MAKE_COMMAND} install)

ExternalProject_Add(FLTK
  SVN_REPOSITORY "http://svn.easysw.com/public/fltk/fltk/branches/branch-1.3"
  SVN_REVISION -r "9815"
  UPDATE_COMMAND ""
  PATCH_COMMAND ${FLTK_PATCH_COMMAND}
  SOURCE_DIR FLTK
  BINARY_DIR FLTK-build
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    ${ep_common_args}
    -DOPTION_BUILD_EXAMPLES:BOOL=OFF
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/FLTK-install
)

if (WIN32)
  set(FLTK_DIR ${CMAKE_BINARY_DIR}/FLTK-install/CMake)
elseif(APPLE)
  set(FLTK_DIR ${CMAKE_BINARY_DIR}/FLTK-install/FLTK/.framework/Resources/CMake)
else()
  set(FLTK_DIR ${CMAKE_BINARY_DIR}/FLTK-install/lib/FLTK-1.3)
endif()

set(FLUID_COMMAND ${CMAKE_BINARY_DIR}/FLTK-install/bin/fluid)
