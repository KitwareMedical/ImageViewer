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

SOURCE_FILES(ImageRegistration_SRCS
liImageRegistrationConsole
liImageRegistrationConsoleGUI
liImageRegistrationConsoleBase
ImageRegistration
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

IF(WIN32)
  LINK_LIBRARIES(
  wsock32
  opengl32
  )
ENDIF(WIN32)

IF(UNIX)
  LINK_LIBRARIES (
  -lm
  )
ENDIF(UNIX)

ADD_EXECUTABLE(ImageRegistration ImageRegistration_SRCS)

