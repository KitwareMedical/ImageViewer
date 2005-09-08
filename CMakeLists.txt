PROJECT(ImageViewer)

IF( BUILD_OUTSIDE_INSIGHT_APPLICATIONS )

  FIND_PACKAGE(ITK)
  IF(ITK_FOUND)
    INCLUDE(${ITK_USE_FILE})
  ELSE(ITK_FOUND)
    MESSAGE(FATAL_ERROR
            "Cannot build InsightApplications without ITK.  Please set ITK_DIR.")
  ENDIF(ITK_FOUND)

  FIND_PACKAGE(FLTK)
  IF(FLTK_FOUND)
    INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})
  ENDIF(FLTK_FOUND)

ENDIF( BUILD_OUTSIDE_INSIGHT_APPLICATIONS )

INCLUDE_DIRECTORIES (
  ${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
)

LINK_LIBRARIES (
  ITKBasicFilters
  ITKIO
  ITKFltkImageViewer
)


SET(ImageViewer_GUI_SRCS
  ImageViewerGUI.fl
  )

FLTK_WRAP_UI( ImageViewer ${ImageViewer_GUI_SRCS} )
ITK_DISABLE_FLTK_GENERATED_WARNINGS("${ImageViewer_GUI_SRCS}")

SET ( ImageViewer_SRCS
  ImageViewer.cxx
  ${ImageViewer_FLTK_UI_SRCS}
  )

ADD_GUI_EXECUTABLE( ImageViewer "${ImageViewer_SRCS}" )
