# Add an option to build or not build the FltkImageViewer Library
IF(HAS_FLTK)
  OPTION(BUILD_FLTKIMAGEVIEWER "Build the FltkImageViewer Library" ON)
  IF(BUILD_FLTKIMAGEVIEWER)
    SET(HAS_FLTKIMAGEVIEWER 1 CACHE INTERNAL "FLTK image support")
    SUBDIRS(FltkImageViewer)
  ELSE(BUILD_FLTKIMAGEVIEWER)
    SET(HAS_FLTKIMAGEVIEWER 0 CACHE INTERNAL "no FLTK image support")
  ENDIF(BUILD_FLTKIMAGEVIEWER)
ENDIF(HAS_FLTK)

# Add an option to build or not build the vtk interface Library
IF(HAS_VTK)
  OPTION(BUILD_VTKINTERFACE "Build the VTK interface Library")
  IF(BUILD_VTKINTERFACE)
    SUBDIRS(vtk)
  ENDIF(BUILD_VTKINTERFACE)
ENDIF(HAS_VTK)


SET(HAS_VTK_FLTK_WINDOW_INTERACTOR 0 CACHE INTERNAL "no vtkFlWindowInteractor image support")
IF(HAS_VTK)
  IF(HAS_FLTK)
    SET(HAS_VTK_FLTK_WINDOW_INTERACTOR 1 CACHE INTERNAL "vtkFlWindowInteractor image support")
    SUBDIRS(VtkFltk)
  ENDIF(HAS_FLTK)
ENDIF(HAS_VTK)
