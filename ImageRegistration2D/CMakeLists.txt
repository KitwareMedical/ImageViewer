# Include OpenGL
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)


INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/ImageRegistration2D
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Examples/ImageRegistration2D
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Examples/ImageRegistration2D
)

SOURCE_FILES(ImageRegistration2D_SRCS
ImageRegistrationConsole.cxx
ImageRegistrationConsoleBase.cxx
ImageRegistration2D.cxx
)

SOURCE_FILES(ImageRegistration2D_GUI_SRCS
ImageRegistrationConsoleGUI.fl
)

LINK_LIBRARIES (
VXLNumerics
ITKCommon
ITKNumerics
VXLNumerics
ITKBasicFilters 
ITKIO
MetaIO
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
  ${CMAKE_X_LIBS} -lm
  )
ENDIF(UNIX)

ADD_EXECUTABLE(ImageRegistration2D ImageRegistration2D_SRCS)

FLTK_WRAP_UI( ImageRegistration2D ImageRegistration2D_GUI_SRCS )

