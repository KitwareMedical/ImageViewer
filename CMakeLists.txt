# Add an option to build or not build the MetaImage Library
OPTION(BUILD_METAIMAGE "Build the MetaImage Library")
IF(BUILD_METAIMAGE)
  SET(HAS_METAIMAGE 1 CACHE INTERNAL "meta image support")
  SUBDIRS(MetaImage)
ELSE(BUILD_METAIMAGE)
  SET(HAS_METAIMAGE 0 CACHE INTERNAL "no meta image support")
ENDIF(BUILD_METAIMAGE)

# Add an option to build or not build the FltkImageViewer Library
OPTION(BUILD_FLTKIMAGEVIEWER "Build the FltkImageViewer Library")
IF(BUILD_FLTKIMAGEVIEWER)
  SET(HAS_FLTKIMAGEVIEWER 1 CACHE INTERNAL "FLTK image support")
  SUBDIRS(FltkImageViewer)
ELSE(BUILD_FLTKIMAGEVIEWER)
  SET(HAS_FLTKIMAGEVIEWER 0 CACHE INTERNAL "no FLTK image support")
ENDIF(BUILD_FLTKIMAGEVIEWER)

# Add an option to build or not build the vtk interface Library
OPTION(BUILD_VTKINTERFACE "Build the VTK interface Library")
IF(BUILD_VTKINTERFACE)
SUBDIRS(vtk)
ENDIF(BUILD_VTKINTERFACE)


