PROJECT( SurfaceGenerator )

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${SurfaceGenerator_SOURCE_DIR}
${SurfaceGenerator_BINARY_DIR}
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
${GLU_INCLUDE_PATH}
)

SET(SurfaceGenerator_SRCS
CriticalPoint.cxx
SurfaceGeneratorBase.cxx
SurfaceGenerator.cxx
main.cxx
)

SET(SurfaceGenerator_GUI_SRCS
SurfaceGeneratorGUI.fl
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


ADD_EXECUTABLE(SurfaceGenerator ${SurfaceGenerator_SRCS})

FLTK_WRAP_UI(SurfaceGenerator  ${SurfaceGenerator_GUI_SRCS})

