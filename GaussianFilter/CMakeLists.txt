FIND_INCLUDE( FLTK_INCLUDE_DIR    FL/Fl.H     H:/usr/local/fltk /usr/local/fltk /usr/local/include )
FIND_LIBRARY( FLTK_LIBRARY_DIR    fltk.lib    H:/usr/local/fltk/lib /usr/local/fltk/lib /usr/local/lib )

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Unsupported/MetaImage
${ITK_SOURCE_DIR}/Unsupported/MetaImage/IView
${ITK_SOURCE_DIR}/Unsupported/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/GaussianFilter
${FLTK_INCLUDE_DIR}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Unsupported/MetaImage
${ITK_BINARY_DIR}/Unsupported/FltkImageViewer
${ITK_BINARY_DIR}/Unsupported/MetaImage/IView
${ITK_BINARY_DIR}/Examples/GaussianFilter
${FLTK_LIBRARY_DIR}
)

SOURCE_FILES(GaussianFilter_SRCS
liFilterConsole
liFilterConsoleGUI
liFilterConsoleBase
GaussianFilter
)

LINK_LIBRARIES (
ITKCommon
ITKBasicFilters 
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


ADD_EXECUTABLE(GaussianFilter GaussianFilter_SRCS)

