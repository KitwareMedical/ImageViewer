PROJECT( ImageRegistration )

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${ImageRegistration_SOURCE_DIR}
${ImageRegistration_BINARY_DIR}
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

SET(ImageRegistration_SRCS
liImageRegistrationConsole.cxx
liImageRegistrationConsoleBase.cxx
ImageRegistration.cxx
)

SET(ImageRegistration_GUI_SRCS
liImageRegistrationConsoleGUI.fl
)

LINK_LIBRARIES (
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
  ${CMAKE_X_LIBS}
  )
ENDIF(UNIX)

ADD_EXECUTABLE(ImageRegistration ${ImageRegistration_SRCS})

FLTK_WRAP_UI( ImageRegistration ${ImageRegistration_GUI_SRCS} )

