
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


ADD_GUI_EXECUTABLE( ImageViewer "${ImageViewer_SRCS}" )

SET(ImageViewer_GUI_SRCS
  ImageViewerGUI.fl
  )

FLTK_WRAP_UI( ImageViewer ${ImageViewer_GUI_SRCS} )
ITK_DISABLE_FLTK_GENERATED_WARNINGS("${ImageViewer_GUI_SRCS}")
