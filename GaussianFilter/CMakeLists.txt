PROJECT( GaussianFilter )

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/IO
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${GaussianFilter_SOURCE_DIR}
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Code/IO
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${GaussianFilter_BINARY_DIR}
)

SET(GaussianFilter_SRCS
liFilterConsole.cxx
liFilterConsoleBase.cxx
GaussianFilter.cxx
)

SET(GaussianFilter2D_SRCS
liFilterConsole2D.cxx
liFilterConsole2DBase.cxx
GaussianFilter2D.cxx
)

SET(GaussianFilter_GUI_SRCS
liFilterConsoleGUI.fl
)

SET(GaussianFilter2D_GUI_SRCS
liFilterConsole2DGUI.fl
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


ADD_EXECUTABLE(GaussianFilter   ${GaussianFilter_SRCS})
ADD_EXECUTABLE(GaussianFilter2D ${GaussianFilter2D_SRCS})

FLTK_WRAP_UI(GaussianFilter     ${GaussianFilter_GUI_SRCS})
FLTK_WRAP_UI(GaussianFilter2D   ${GaussianFilter2D_GUI_SRCS})



