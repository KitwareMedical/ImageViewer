INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/DistanceMapFilter
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Examples/DistanceMapFilter
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
${GLU_INCLUDE_PATH}
)

SET(DistanceMapFilter_SRCS
DistanceMapFilterBase.cxx
DistanceMapFilter.cxx
main.cxx
)

SET(DistanceMapFilter_GUI_SRCS
DistanceMapFilterGUI.fl
)

LINK_LIBRARIES (
ITKBasicFilters 
ITKIO
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
  )
ENDIF(UNIX)

ADD_EXECUTABLE(DistanceMapFilter ${DistanceMapFilter_SRCS})

FLTK_WRAP_UI(DistanceMapFilter  ${DistanceMapFilter_GUI_SRCS})




