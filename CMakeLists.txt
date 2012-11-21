cmake_minimum_required( VERSION 2.8 )

project( ImageViewer )

find_package( ITK REQUIRED )
include( ${ITK_USE_FILE} )

find_package( FLTK REQUIRED NO_MODULE )
set(FLTK_EXE_LINKER_FLAGS "")
include( ${FLTK_USE_FILE} )
link_directories( ${FLTK_LIB_DIR} )
set(FLTK_LIBRARIES "fltk;fltk_images;fltk_forms;fltk_gl")
if(APPLE)
  list(APPEND FLTK_LIBRARIES "-framework Cocoa")
endif()

# The fluid-generated fltk sources have many warnings.  This macro
# will disable warnings for the generated files on some compilers.
macro(ITK_DISABLE_FLTK_GENERATED_WARNINGS files)
  if(CMAKE_COMPILER_IS_GNUCXX)
    foreach(f ${files})
      string(REGEX REPLACE "\\.fl$" ".cxx" SRC "${f}")
      string(REGEX REPLACE ".*/([^/]*)$" "\\1" SRC "${SRC}")
      set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS -w)
    endforeach(f)
  endif(CMAKE_COMPILER_IS_GNUCXX)
  if(MSVC)
    foreach(f ${files})
      string(REGEX REPLACE "\\.fl$" ".cxx" SRC "${f}")
      string(REGEX REPLACE ".*/([^/]*)$" "\\1" SRC "${SRC}")
      set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS /w)
    endforeach(f)
  endif(MSVC)
endmacro(ITK_DISABLE_FLTK_GENERATED_WARNINGS)

# Macro for adding resources to GUI application on the Mac
#
if(APPLE AND NOT FLTK_USE_X)
    find_program(ITK_APPLE_RESOURCE Rez /Developer/Tools)
    find_file(ITK_FLTK_RESOURCE mac.r
      /usr/local/include/FL
      ${FLTK_DIR}/FL)
    if(NOT ITK_FLTK_RESOURCE)
      message("Fltk resources not found, GUI application will not respond to mouse events")
    endif(NOT ITK_FLTK_RESOURCE)

    macro(ADD_GUI_EXECUTABLE name sources)
      add_executable(${name} ${sources})
      install_targets(/bin ${name})
      set(EXEC_PATH ${EXECUTABLE_OUTPUT_PATH})
      if(NOT EXEC_PATH)
        set(EXEC_PATH ${CMAKE_CURRENT_BINARY_DIR})
      endif(NOT EXEC_PATH)
        if(ITK_APPLE_RESOURCE)
          add_custom_command(SOURCE ${name}
                             COMMAND ${ITK_APPLE_RESOURCE}
                             ARGS -t APPL ${ITK_FLTK_RESOURCE} -o
                             ${EXEC_PATH}/${name}
                             TARGET ${name})
        endif(ITK_APPLE_RESOURCE)
    endmacro(ADD_GUI_EXECUTABLE)
else(APPLE AND NOT FLTK_USE_X)
  macro(ADD_GUI_EXECUTABLE name sources)
    add_executable(${name} ${sources})
    install_targets(/bin ${name})
  endmacro(ADD_GUI_EXECUTABLE)
endif(APPLE AND NOT FLTK_USE_X)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/FltkImageViewer )

# Creates the library.
add_subdirectory( FltkImageViewer )
# Creates the executable.
add_subdirectory( ImageViewer )
