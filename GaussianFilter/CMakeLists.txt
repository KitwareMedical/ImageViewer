INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Unsupported/MetaImage
${ITK_SOURCE_DIR}/Unsupported/MetaImage/IView
${ITK_SOURCE_DIR}/Unsupported/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/GaussianFilter
${FLTK_SOURCE_DIR}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Unsupported/MetaImage
${ITK_BINARY_DIR}/Unsupported/FltkImageViewer
${ITK_BINARY_DIR}/Unsupported/MetaImage/IView
${FLTK_BINARY_DIR}
)

SOURCE_FILES(
liCommandUpdateLightButton
liCommandUpdateProgress
liFilterConsole
liFilterConsoleGUI
liFilterConsoleBase
)

LIBRARY(
GaussianFilterLib
)


LINK_LIBRARIES (
VXLNumerics
ITKCommon
ITKBasicFilters 
MetaImageIO
IView
FltkImageViewer
fltk
)

UNIX_LIBRARIES (
-lm
)


EXECUTABLES(GaussianFilter)

