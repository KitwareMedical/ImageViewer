#
# Build shared libs ?
#
# Wrap Tcl, Java, Python
#
# Rational: even if your VTK was wrapped, it does not mean that you want to 
# wrap your own local classes. 
# Default value is OFF as the VTK cache might have set them to ON but 
# the wrappers might not be present (or yet not found).
#

#
# Tcl
# 

IF (VTK_WRAP_TCL)
  OPTION(WST_WRAP_TCL 
         "Wrap classes into the TCL interpreted language." 
         OFF)

  IF (WST_WRAP_TCL)

    IF (NOT VTK_WRAP_TCL_EXE)

      MESSAGE("Error. Unable to find VTK_WRAP_TCL_EXE, please edit this value to specify the correct location of the VTK Tcl wrapper.")
      MARK_AS_ADVANCED(CLEAR VTK_WRAP_TCL_EXE)
      SET (WST_CAN_BUILD 0 CACHE INTERNAL)

    ELSE (NOT VTK_WRAP_TCL_EXE)

      FIND_FILE (VTK_WRAP_HINTS hints Wrapping)
      MARK_AS_ADVANCED(VTK_WRAP_HINTS)
      INCLUDE (${CMAKE_ROOT}/Modules/FindTCL.cmake)

      IF (TCL_INCLUDE_PATH)
        INCLUDE_DIRECTORIES(${TCL_INCLUDE_PATH})
      ENDIF (TCL_INCLUDE_PATH)

    ENDIF (NOT VTK_WRAP_TCL_EXE)

  SUBDIR_DEPENDS(Wrapping/Tcl Common)
  ENDIF (WST_WRAP_TCL)
ENDIF (VTK_WRAP_TCL)
