INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Unsupported/MetaImage
${ITK_SOURCE_DIR}/Unsupported/MetaImage/IView
${ITK_SOURCE_DIR}/Unsupported/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/GaussianFilter
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Unsupported/MetaImage
${ITK_BINARY_DIR}/Unsupported/FltkImageViewer
${ITK_BINARY_DIR}/Unsupported/MetaImage/IView
${ITK_BINARY_DIR}/Examples/GaussianFilter
)

SOURCE_FILES(GaussianFilter_SRCS
liFilterConsole
liFilterConsoleGUI
liFilterConsoleBase
GaussianFilter
)

LINK_LIBRARIES (
ITKCommon
ITKBasicFilters 
MetaImageIO
IView
FltkImageViewer
${FLTK_LIBRARY}
${OPENGL_LIBRARY}
)

IF(WIN32)
  LINK_LIBRARIES(
  wsock32
  )
ENDIF(WIN32)

IF(UNIX)
  LINK_LIBRARIES (
  -lm
  )
ENDIF(UNIX)

ADD_EXECUTABLE(GaussianFilter GaussianFilter_SRCS)

