PROJECT( ImageRegistration )

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${ImageRegistration_SOURCE_DIR}
${ImageRegistration_BINARY_DIR}
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
)

ADD_EXECUTABLE(ImageRegistration ${ImageRegistration_SRCS})

FLTK_WRAP_UI( ImageRegistration ${ImageRegistration_GUI_SRCS} )

