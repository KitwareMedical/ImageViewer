
SET ( ImageViewer_SRCS
  ImageViewer.cxx
  )

INCLUDE_DIRECTORIES (
  ${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
)

LINK_LIBRARIES (
  ITKBasicFilters
  ITKIO
  ITKFltkImageViewer
)

IF(WIN32)
  LINK_LIBRARIES(
    wsock32
  )
ENDIF(WIN32)

ADD_GUI_EXECUTABLE( ImageViewer "${ImageViewer_SRCS}" )

SET(ImageViewer_GUI_SRCS
  ImageViewerGUI.fld
  )

FLTK_WRAP_UI( ImageViewer ${ImageViewer_GUI_SRCS} )
ITK_DISABLE_FLTK_GENERATED_WARNINGS("${ImageViewer_GUI_SRCS}")
