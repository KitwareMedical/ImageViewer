INCLUDE_DIRECTORIES(
  ${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_SOURCE_DIR}/Auxiliary/VtkFltk
  ${InsightApplications_SOURCE_DIR}/ThinPlateSplines
  ${InsightApplications_SOURCE_DIR}/ThinPlateSplines
  ${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
)

ADD_EXECUTABLE(ThinPlateSplines ThinPlateSplinesApplicationBase.cxx
               ThinPlateSplinesApplication.cxx main.cxx)
FLTK_WRAP_UI(ThinPlateSplines ThinPlateSplinesApplicationGUI.fl)
TARGET_LINK_LIBRARIES(ThinPlateSplines ITKBasicFilters ITKIO
                      ITKFltkImageViewer ITKVtkFltk vtkHybrid)
