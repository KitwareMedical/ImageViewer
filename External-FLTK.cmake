#-----------------------------------------------------------------------------
# Get and build fltk
#
#-----------------------------------------------------------------------------
set(FLTK_INSTALL_COMMAND ${CMAKE_MAKE_COMMAND} install)

if(CMAKE_COMPILER_IS_GNUCXX)
  set(EXTRA_LIBRARIES "-DCMAKE_EXE_LINKER_FLAGS:STRING=-ldl")
endif()

ExternalProject_Add(FLTK
  SVN_REPOSITORY "http://svn.easysw.com/public/fltk/fltk/branches/branch-1.3"
  SVN_REVISION ""
  UPDATE_COMMAND ""
  SOURCE_DIR FLTK
  BINARY_DIR FLTK-build
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
    ${ep_common_args}
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/FLTK-install
    ${EXTRA_LIBRARIES}
)
set(FLTK_DIR ${CMAKE_BINARY_DIR}/FLTK-install/lib/FLTK-1.3)
set(FLUID_COMMAND ${CMAKE_BINARY_DIR}/FLTK-install/bin/fluid)
