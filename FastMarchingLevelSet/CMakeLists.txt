INCLUDE_DIRECTORIES(
  ${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_SOURCE_DIR}/Auxiliary/VtkFltk
  ${InsightApplications_SOURCE_DIR}/Auxiliary/vtk
  ${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_BINARY_DIR}/Auxiliary/VtkFltk
  ${InsightApplications_SOURCE_DIR}/FastMarchingLevelSet
  ${InsightApplications_BINARY_DIR}/FastMarchingLevelSet
)

SET(FastMarchingLevelSet_SRCS 
  FastMarchingLevelSetBase.cxx
  FastMarchingLevelSet.cxx
  )

ADD_GUI_EXECUTABLE(FastMarchingLevelSet "${FastMarchingLevelSet_SRCS}")

FLTK_WRAP_UI(FastMarchingLevelSet FastMarchingLevelSetGUI.fl)

TARGET_LINK_LIBRARIES(FastMarchingLevelSet ITKFltkImageViewer ITKVtkFltk
                      ITKBasicFilters ITKNumerics ITKIO)
