INCLUDE_DIRECTORIES(
  ${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_SOURCE_DIR}/GaussianFilter
  ${InsightApplications_BINARY_DIR}/GaussianFilter
)

SET(GaussianFilter_SRCS 
  liFilterConsole.cxx
  liFilterConsoleBase.cxx 
  GaussianFilter.cxx
  )

SET(GaussianFilter2D_SRCS 
  liFilterConsole2D.cxx
  liFilterConsole2DBase.cxx 
  GaussianFilter2D.cxx
  )

ADD_GUI_EXECUTABLE(GaussianFilter   "${GaussianFilter_SRCS}")
ADD_GUI_EXECUTABLE(GaussianFilter2D "${GaussianFilter2D_SRCS}")

FLTK_WRAP_UI(GaussianFilter liFilterConsoleGUI.fl)
FLTK_WRAP_UI(GaussianFilter2D liFilterConsole2DGUI.fl)

TARGET_LINK_LIBRARIES(GaussianFilter ITKFltkImageViewer ITKIO ITKBasicFilters)
TARGET_LINK_LIBRARIES(GaussianFilter2D ITKFltkImageViewer ITKIO ITKBasicFilters)
