# Add an option to build or not build the FltkImageViewer Library
IF(FLTK_FOUND)
  SUBDIRS(FltkImageViewer)
  IF(VTK_FOUND)
    SUBDIRS(VtkFltk)
  ENDIF(VTK_FOUND)
ENDIF(FLTK_FOUND)

# Add an option to build or not build the vtk interface Library
IF(VTK_FOUND)
  SUBDIRS(vtk)
ENDIF(VTK_FOUND)
