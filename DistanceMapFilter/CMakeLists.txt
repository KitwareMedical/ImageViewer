INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Utilities/zlib
${ITK_SOURCE_DIR}/Utilities/png
${ITK_SOURCE_DIR}/Utilities/MetaIO
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/DistanceMapFilter
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Examples/DistanceMapFilter
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
${GLU_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Code/IO
${ITK_BINARY_DIR}/Utilities/png
${ITK_BINARY_DIR}/Utilities/zlib
${ITK_BINARY_DIR}/Utilities/MetaIO
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Examples/DistanceMapFilter
)

SOURCE_FILES(DistanceMapFilter_SRCS
DistanceMapFilterBase
DistanceMapFilter
main
)

SOURCE_FILES(DistanceMapFilter_GUI_SRCS
DistanceMapFilterGUI.fl
)

LINK_LIBRARIES (
ITKCommon
ITKBasicFilters 
ITKIO
MetaIO
itkpng
itkzlib
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
  ${CMAKE_X_LIBS} 
  -lm
  )
ENDIF(UNIX)

ADD_EXECUTABLE(DistanceMapFilter DistanceMapFilter_SRCS)

FLTK_WRAP_UI(DistanceMapFilter  DistanceMapFilter_GUI_SRCS)




