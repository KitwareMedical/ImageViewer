PROJECT (SimpleFuzzyConnectedness)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
${SimpleFuzzyConnectedness_SOURCE_DIR}
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
FuzzySegMain.cpp
FuzzySegGrayRun.cxx
)

SET(Project_FLTK_SRCS
FuzzySegGrayUI.fl
)

ADD_EXECUTABLE(SimpleFuzzyConnectedness   ${Project_SRCS})

IF(HAS_FLTK)
FLTK_WRAP_UI(SimpleFuzzyConnectedness ${Project_FLTK_SRCS})
ENDIF(HAS_FLTK)

