# Include OpenGL
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)


INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Auxiliary/MetaImage
${ITK_SOURCE_DIR}/Auxiliary/MetaImage/IView
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/ImageRegistration2D
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Examples/ImageRegistration2D
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
${ITK_BINARY_DIR}/Examples/ImageRegistration2D
)

SOURCE_FILES(ImageRegistration2D_SRCS
ImageRegistrationConsole
ImageRegistrationConsoleBase
ImageRegistration2D
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
IView
FltkImageViewer
${FLTK_LIBRARY}
${OPENGL_LIBRARY}
${GLU_LIBRARY}
)

ADD_EXECUTABLE(ImageRegistration2D ImageRegistration2D_SRCS)

FLTK_WRAP_UI( ImageRegistration2D ImageRegistration2D_GUI_SRCS )

