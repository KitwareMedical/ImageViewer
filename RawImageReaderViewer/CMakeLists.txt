
INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/RawImageReaderViewer
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

LINK_LIBRARIES (
ITKBasicFilters 
ITKIO
ITKFltkImageViewer
${FLTK_LIBRARY}
${OPENGL_LIBRARY}
${GLU_LIBRARY}
)

IF(WIN32)
  LINK_LIBRARIES(
  wsock32
  )
ENDIF(WIN32)

IF(UNIX)
  LINK_LIBRARIES (
  ${CMAKE_X_LIBS} -lm
  )
ENDIF(UNIX)


SET(RawImageReaderViewer_SRCS
itkRawImageReaderViewer.cxx
RawImageReaderViewer.cxx
)

SET(RawImageReaderViewer_GUI_SRCS
itkRawImageReaderViewerGUI.fl
)

ADD_EXECUTABLE(RawImageReaderViewer   ${RawImageReaderViewer_SRCS})

FLTK_WRAP_UI(RawImageReaderViewer   ${RawImageReaderViewer_GUI_SRCS})


