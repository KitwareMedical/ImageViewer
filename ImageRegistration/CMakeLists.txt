# Include FLTK
INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)

# Include OpenGL
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)


INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Auxiliary/MetaImage
${ITK_SOURCE_DIR}/Auxiliary/MetaImage/IView
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/ImageRegistration
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Examples/ImageRegistration
${ITK_BINARY_DIR}/Auxiliary/MetaImage/IView
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Auxiliary/MetaImage
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Auxiliary/MetaImage/IView
${ITK_BINARY_DIR}/Examples/ImageRegistration
)

SOURCE_FILES(ImageRegistration_SRCS
liImageRegistrationConsole
liImageRegistrationConsoleBase
ImageRegistration
)

SOURCE_FILES(ImageRegistration_GUI_SRCS
liImageRegistrationConsoleGUI.fl
)

LINK_LIBRARIES (
VXLNumerics
ITKCommon
ITKBasicFilters 
MetaImageIO
IView
FltkImageViewer
${FLTK_LIBRARY}
${OPENGL_LIBRARY}
)

IF(WIN32)
  LINK_LIBRARIES(
  wsock32
  )
ENDIF(WIN32)

IF(UNIX)
  LINK_LIBRARIES (
  -lm
  )
ENDIF(UNIX)

ADD_EXECUTABLE(ImageRegistration ImageRegistration_SRCS)

FLTK_WRAP_UI( ImageRegistration ImageRegistration_GUI_SRCS )

