PROJECT( RawImageReaderViewer )

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${RawImageReaderViewer_SOURCE_DIR}
)

LINK_LIBRARIES (
ITKBasicFilters 
ITKIO
ITKFltkImageViewer
)


SET(RawImageReaderViewer_SRCS
itkRawImageReaderViewer.cxx
RawImageReaderViewer.cxx
)

SET(RawImageReaderViewer_GUI_SRCS
itkRawImageReaderViewerGUI.fl
)

ADD_GUI_EXECUTABLE(RawImageReaderViewer   "${RawImageReaderViewer_SRCS}")

FLTK_WRAP_UI(RawImageReaderViewer   ${RawImageReaderViewer_GUI_SRCS})
