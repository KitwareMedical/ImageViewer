INCLUDE_DIRECTORIES(
  ${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_SOURCE_DIR}/AnisotropicDiffusionFltkGui
  ${InsightApplications_BINARY_DIR}/AnisotropicDiffusionFltkGui
)

SET(AnisotropicDiffusionFltkGui3D_SRCS
liFilterConsole3D.cxx
liFilterConsole3DBase.cxx
Filters3D.cxx
)

SET(AnisotropicDiffusionFltkGui2D_SRCS
liFilterConsole2D.cxx
liFilterConsole2DBase.cxx
Filters2D.cxx
)

SET(AnisotropicDiffusionFltkGui3D_GUI_SRCS
liFilterConsole3DGUI.fl
)

SET(AnisotropicDiffusionFltkGui2D_GUI_SRCS
liFilterConsole2DGUI.fl
)

ADD_GUI_EXECUTABLE(AnisotropicDiffusionFltkGui3D "${AnisotropicDiffusionFltkGui3D_SRCS}")
ADD_GUI_EXECUTABLE(AnisotropicDiffusionFltkGui2D "${AnisotropicDiffusionFltkGui2D_SRCS}")

FLTK_WRAP_UI(AnisotropicDiffusionFltkGui3D   ${AnisotropicDiffusionFltkGui3D_GUI_SRCS})
FLTK_WRAP_UI(AnisotropicDiffusionFltkGui2D   ${AnisotropicDiffusionFltkGui2D_GUI_SRCS})

TARGET_LINK_LIBRARIES(AnisotropicDiffusionFltkGui2D ITKFltkImageViewer ITKIO
                      ITKBasicFilters)
TARGET_LINK_LIBRARIES(AnisotropicDiffusionFltkGui3D ITKFltkImageViewer ITKIO
                      ITKBasicFilters)
