# Include OpenGL
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)

INCLUDE_REGULAR_EXPRESSION(".*")

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/MutualInformationEuler2DRegistration
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Examples/MutualInformationEuler2DRegistration
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)


LINK_LIBRARIES (
VXLNumerics
ITKNumerics
ITKCommon
ITKIO
MetaIO
itkpng
itkzlib
ITKBasicFilters 
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

SET(MutualInformationEuler2DRegistration_SRCS
RegisterApplication.cxx
RegisterWindow.cxx
register.cxx
)

SET(MutualInformationEuler2DRegistration_GUI_SRCS
registerGUI.fl
)

ADD_EXECUTABLE(MutualInformationEuler2DRegistration ${MutualInformationEuler2DRegistration_SRCS})
FLTK_WRAP_UI(MutualInformationEuler2DRegistration ${MutualInformationEuler2DRegistration_GUI_SRCS})
