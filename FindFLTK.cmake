#
# Find the native FLTK includes and library
#
# The following settings are defined
# FLTK_FLUID_EXECUTABLE, where to find the Fluid tool
# FLTK_WRAP_UI, This allows the FLTK_WRAP_UI command to work.
# FLTK_INCLUDE_DIR, where to find include files
# FLTK_LIBRARIES, list of fltk libraries
# FLTK_FOUND, Don't use FLTK if false.

# The following settings should not be used in general.
# FLTK_BASE_LIBRARY    = the full path to fltk.lib
# FLTK_GL_LIBRARY      = the full path to fltk_gl.lib
# FLTK_FORMS_LIBRARY   = the full path to fltk_forms.lib
# FLTK_IMAGES_LIBRARY  = the full path to fltk_images.lib

# Platform dependent libraries required by FLTK
IF(WIN32)
  IF(NOT CYGWIN)
    IF(BORLAND)
      SET( FLTK_PLATFORM_DEPENDENT_LIBS import32 )
    ELSE(BORLAND)
      SET( FLTK_PLATFORM_DEPENDENT_LIBS wsock32 comctl32 )
    ENDIF(BORLAND)
  ENDIF(NOT CYGWIN)
ENDIF(WIN32)

IF(UNIX)
  INCLUDE(${CMAKE_ROOT}/Modules/FindX11.cmake)
  SET( FLTK_PLATFORM_DEPENDENT_LIBS ${X11_LIBRARIES} -lm)
ENDIF(UNIX)

IF(APPLE)
  SET( FLTK_PLATFORM_DEPENDENT_LIBS  "-framework Carbon -framework Cocoa -framework ApplicationServices -lz")
ENDIF(APPLE)

# Find fluid executable.
FIND_PROGRAM(FLTK_FLUID_EXECUTABLE fluid ${FLTK_INCLUDE_DIR}/fluid)

# Use location of fluid to help find everything else.
SET(FLTK_INCLUDE_SEARCH_PATH "")
SET(FLTK_LIBRARY_SEARCH_PATH "")
IF(FLTK_FLUID_EXECUTABLE)
  GET_FILENAME_COMPONENT(FLTK_BIN_DIR "${FLTK_FLUID_EXECUTABLE}" PATH)
  SET(FLTK_INCLUDE_SEARCH_PATH ${FLTK_INCLUDE_SEARCH_PATH}
      ${FLTK_BIN_DIR}/../include)
  SET(FLTK_LIBRARY_SEARCH_PATH ${FLTK_LIBRARY_SEARCH_PATH}
      ${FLTK_BIN_DIR}/../lib)
  SET(FLTK_WRAP_UI 1)
ENDIF(FLTK_FLUID_EXECUTABLE)

SET(FLTK_INCLUDE_SEARCH_PATH ${FLTK_INCLUDE_SEARCH_PATH}
  /usr/local/include
  /usr/include
  /usr/local/fltk
  /usr/X11R6/include
)

FIND_PATH(FLTK_INCLUDE_DIR FL/Fl.h ${FLTK_INCLUDE_SEARCH_PATH})

SET(FLTK_LIBRARY_SEARCH_PATH ${FLTK_LIBRARY_SEARCH_PATH}
  /usr/lib
  /usr/local/lib
  /usr/local/fltk/lib
  /usr/X11R6/lib
  ${FLTK_INCLUDE_DIR}/lib
)

FIND_LIBRARY(FLTK_BASE_LIBRARY NAMES fltk fltkd
             PATHS ${FLTK_LIBRARY_SEARCH_PATH})
FIND_LIBRARY(FLTK_GL_LIBRARY NAMES fltkgl fltkgld fltk_gl
             PATHS ${FLTK_LIBRARY_SEARCH_PATH})
FIND_LIBRARY(FLTK_FORMS_LIBRARY NAMES fltkforms fltkformsd fltk_forms
             PATHS ${FLTK_LIBRARY_SEARCH_PATH})
FIND_LIBRARY(FLTK_IMAGES_LIBRARY NAMES fltkimages fltkimagesd fltk_images
             PATHS ${FLTK_LIBRARY_SEARCH_PATH})

SET(FLTK_FOUND 1)
FOREACH(var FLTK_FLUID_EXECUTABLE FLTK_INCLUDE_DIR
            FLTK_BASE_LIBRARY FLTK_GL_LIBRARY
            FLTK_FORMS_LIBRARY FLTK_IMAGES_LIBRARY)
  IF(NOT ${var})
    SET(FLTK_FOUND 0)
  ENDIF(NOT ${var})
ENDFOREACH(var)

IF(FLTK_FOUND)
  SET(FLTK_LIBRARIES ${FLTK_BASE_LIBRARY} ${FLTK_GL_LIBRARY}
                     ${FLTK_FORMS_LIBRARY} ${FLTK_IMAGES_LIBRARY}
                     ${FLTK_PLATFORM_DEPENDENT_LIBS})
ENDIF(FLTK_FOUND)
