INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Unsupported/FltkImageViewer
${ITK_SOURCE_DIR}/Unsupported/MetaImage
${ITK_SOURCE_DIR}/Utilities/zlib
${ITK_SOURCE_DIR}/Utilities/png
${ITK_SOURCE_DIR}/Examples/Curves2DExtractor
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Code/IO
${ITK_BINARY_DIR}/Unsupported/FltkImageViewer
${ITK_BINARY_DIR}/Unsupported/MetaImage
${ITK_BINARY_DIR}/Examples/Curves2DExtractor
${ITK_BINARY_DIR}/Utilities/png
${ITK_BINARY_DIR}/Utilities/zlib
)


SOURCE_FILES(Curves2DExtractor_SRCS
ceExtractorConsole
ceExtractorConsoleBase
)

SOURCE_FILES(FLTK_GUI_Files_SRCS
ceExtractorConsoleGUI.fl
)

#
# Take the list of GUI files, generate .cxx and .h from 
# them and put the generated file in the second source
# list
#
FLTK_WRAP_UI(FLTK_GUI_Files_SRCS Curves2DExtractor_SRCS)

ADD_LIBRARY(Curves2DExtractorLib Curves2DExtractor_SRCS)


LINK_LIBRARIES (
ITKCommon
ITKBasicFilters 
ITKIO
MetaImageIO
png
zlib
FltkImageViewer
Curves2DExtractorLib
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
  -lm
  )
ENDIF(UNIX)

ADD_EXECUTABLE(Curves2DExtractor main)

