#
# Find the FltkImageViewer includes and library
#
# The following settings are defined
# FltkImageViewer_INCLUDE_DIR = Where to find include files
# FltkImageViewer_LIBRARY     = The full path to ITKFltkImageViewer.lib
# FltkImageViewer_FOUND       = Don't use if false.

# The following settings should not be used in general.


FIND_PATH(FltkImageViewer_INCLUDE_DIR fltkImageViewer.h)

FIND_LIBRARY(FltkImageViewer_LIBRARY NAMES ITKFltkImageViewer)

SET(FltkImageViewer_FOUND 1)
FOREACH(var FltkImageViewer_INCLUDE_DIR
            FltkImageViewer_LIBRARY)
  IF(NOT ${var})
    SET(FltkImageViewer_FOUND 0)
  ENDIF(NOT ${var})
ENDFOREACH(var)

