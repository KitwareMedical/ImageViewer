PROJECT (VoronoiSegmentation)

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
${VoronoiSegmentation_SOURCE_DIR}
)

LINK_LIBRARIES (
ITKCommon
ITKNumerics
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


SET(Project_SRCS
VDSegMain.cpp
VDSegGrayRun.cxx
)

SET(Project_FLTK_SRCS
VDSegGrayUI.fl
)

ADD_EXECUTABLE(VoronoiSegmentation   ${Project_SRCS})

IF(HAS_FLTK)
FLTK_WRAP_UI(VoronoiSegmentation ${Project_FLTK_SRCS})
ENDIF(HAS_FLTK)

