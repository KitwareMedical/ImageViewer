FIND_INCLUDE( FLTK_INCLUDE_DIR    FL/Fl.H     H:/usr/local/fltk     )
FIND_LIBRARY( FLTK_LIBRARY_DIR    fltk.lib    H:/usr/local/fltk/lib )

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Unsupported/MetaImage
${ITK_SOURCE_DIR}/Unsupported/MetaImage/IView
${ITK_SOURCE_DIR}/Unsupported/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/ImageRegistration
${FLTK_INCLUDE_DIR}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Unsupported/MetaImage
${ITK_BINARY_DIR}/Unsupported/FltkImageViewer
${ITK_BINARY_DIR}/Unsupported/MetaImage/IView
${ITK_BINARY_DIR}/Examples/ImageRegistration
${FLTK_LIBRARY_DIR}
)

SOURCE_FILES(
liImageRegistrationConsole
liImageRegistrationConsoleGUI
liImageRegistrationConsoleBase
)

LIBRARY(
ImageRegistrationLib
)


LINK_LIBRARIES (
VXLNumerics
ITKCommon
ITKBasicFilters 
ImageRegistrationLib
MetaImageIO
IView
FltkImageViewer
fltk
)

WIN32_LIBRARIES(
wsock32
opengl32
)

UNIX_LIBRARIES (
-lm
)


EXECUTABLES(ImageRegistration)

