# Look for OpenGL
INCLUDE (${ITK_SOURCE_DIR}/Utilities/ITKFindOpenGL.cmake)

# Add an option to build or not build the FltkImageViewer Library
IF(HAS_FLTK)
  SET(HAS_FLTKIMAGEVIEWER 1 CACHE INTERNAL "FLTK image support")
  SUBDIRS(FltkImageViewer)
ENDIF(HAS_FLTK)

# Add an option to build or not build the vtk interface Library
IF(HAS_VTK)
  SUBDIRS(vtk)
ENDIF(HAS_VTK)


SET(HAS_VTK_FLTK_WINDOW_INTERACTOR 0 CACHE INTERNAL "no vtkFlWindowInteractor image support")
IF(HAS_VTK)
  IF(HAS_FLTK)
    SET(HAS_VTK_FLTK_WINDOW_INTERACTOR 1 CACHE INTERNAL "vtkFlWindowInteractor image support")
    SUBDIRS(VtkFltk)
  ENDIF(HAS_FLTK)
ENDIF(HAS_VTK)
