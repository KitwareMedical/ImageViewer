INCLUDE_DIRECTORIES(
  ${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_SOURCE_DIR}/GaussianFilter
  ${InsightApplications_BINARY_DIR}/GaussianFilter
)

ADD_EXECUTABLE(GaussianFilter liFilterConsole.cxx
               liFilterConsoleBase.cxx GaussianFilter.cxx)
ADD_EXECUTABLE(GaussianFilter2D liFilterConsole2D.cxx
               liFilterConsole2DBase.cxx GaussianFilter2D.cxx)

FLTK_WRAP_UI(GaussianFilter liFilterConsoleGUI.fl)
FLTK_WRAP_UI(GaussianFilter2D liFilterConsole2DGUI.fl)

TARGET_LINK_LIBRARIES(GaussianFilter ITKFltkImageViewer ITKIO ITKBasicFilters)
TARGET_LINK_LIBRARIES(GaussianFilter2D ITKFltkImageViewer ITKIO ITKBasicFilters)
