PROJECT( SurfaceGenerator )

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${SurfaceGenerator_SOURCE_DIR}
${SurfaceGenerator_BINARY_DIR}
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
)

IF(WIN32)
  LINK_LIBRARIES(
  wsock32
  )
ENDIF(WIN32)


ADD_EXECUTABLE(SurfaceGenerator ${SurfaceGenerator_SRCS})

FLTK_WRAP_UI(SurfaceGenerator  ${SurfaceGenerator_GUI_SRCS})

