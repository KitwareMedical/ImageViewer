INCLUDE_DIRECTORIES(
  ${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_SOURCE_DIR}/ImageRegistration2D
  ${InsightApplications_BINARY_DIR}/ImageRegistration2D
)

SET(ImageRegistration2D_SRCS 
  ImageRegistrationConsole.cxx
  ImageRegistrationConsoleBase.cxx 
  ImageRegistration2D.cxx
  )

ADD_GUI_EXECUTABLE(ImageRegistration2D "${ImageRegistration2D_SRCS}")

FLTK_WRAP_UI(ImageRegistration2D ImageRegistrationConsoleGUI.fl)
TARGET_LINK_LIBRARIES(ImageRegistration2D ITKFltkImageViewer ITKIO
                      ITKNumerics ITKBasicFilters)
