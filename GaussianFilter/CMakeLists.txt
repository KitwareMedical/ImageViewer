INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Auxiliary/MetaImage/IView
${ITK_SOURCE_DIR}/Auxiliary/MetaImage
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/GaussianFilter
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Examples/GaussianFilter
${ITK_BINARY_DIR}/Auxiliary/MetaImage/IView
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Code/IO
${ITK_BINARY_DIR}/Auxiliary/MetaImage
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Auxiliary/MetaImage/IView
${ITK_BINARY_DIR}/Examples/GaussianFilter
)

SOURCE_FILES(GaussianFilter_SRCS
liFilterConsole
liFilterConsoleBase
GaussianFilter
)

SOURCE_FILES(GaussianFilter_GUI_SRCS
liFilterConsoleGUI.fl
)

LINK_LIBRARIES (
ITKCommon
ITKBasicFilters 
ITKIO
IView
FltkImageViewer
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

ADD_EXECUTABLE(GaussianFilter GaussianFilter_SRCS)

FLTK_WRAP_UI(GaussianFilter GaussianFilter_GUI_SRCS)


