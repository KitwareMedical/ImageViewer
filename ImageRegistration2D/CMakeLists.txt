INCLUDE_DIRECTORIES(
  ${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_SOURCE_DIR}/ImageRegistration2D
  ${InsightApplications_BINARY_DIR}/ImageRegistration2D
)

ADD_EXECUTABLE(ImageRegistration2D ImageRegistrationConsole.cxx
               ImageRegistrationConsoleBase.cxx ImageRegistration2D.cxx)
FLTK_WRAP_UI(ImageRegistration2D ImageRegistrationConsoleGUI.fl)
TARGET_LINK_LIBRARIES(ImageRegistration2D ITKFltkImageViewer ITKIO
                      ITKNumerics ITKBasicFilters)
