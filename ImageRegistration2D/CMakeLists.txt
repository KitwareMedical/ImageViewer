PROJECT( ImageRegistration2D )

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${ImageRegistration2D_SOURCE_DIR}
${ImageRegistration2D_BINARY_DIR}
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

SET(ImageRegistration2D_SRCS
ImageRegistrationConsole.cxx
ImageRegistrationConsoleBase.cxx
ImageRegistration2D.cxx
)

SET(ImageRegistration2D_GUI_SRCS
ImageRegistrationConsoleGUI.fl
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

ADD_EXECUTABLE(ImageRegistration2D ${ImageRegistration2D_SRCS})

FLTK_WRAP_UI( ImageRegistration2D ${ImageRegistration2D_GUI_SRCS} )

