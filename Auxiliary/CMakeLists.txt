# Add an option to build or not build the FltkImageViewer Library
IF(USE_FLTK)
  SET(HAS_FLTKIMAGEVIEWER 1 CACHE INTERNAL "FLTK image support")
  SUBDIRS(FltkImageViewer)
else() 
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/QtImageViewer ) #QtImageViewer

  add_subdirectory( QtImageViewer ) 
ENDIF(USE_FLTK)

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
