INCLUDE_DIRECTORIES(
  ${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_SOURCE_DIR}/Auxiliary/VtkFltk
  ${InsightApplications_SOURCE_DIR}/Auxiliary/vtk
  ${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_BINARY_DIR}/Auxiliary/VtkFltk
  ${InsightApplications_SOURCE_DIR}/GeodesicActiveContour
  ${InsightApplications_BINARY_DIR}/GeodesicActiveContour
)

ADD_EXECUTABLE(GeodesicActiveContour GeodesicActiveContourBase.cxx
               GeodesicActiveContour.cxx)
FLTK_WRAP_UI(GeodesicActiveContour GeodesicActiveContourGUI.fl)

TARGET_LINK_LIBRARIES(GeodesicActiveContour ITKNumerics ITKBasicFilters
                      ITKIO vtkHybrid vtkIO ITKFltkImageViewer ITKVtkFltk)
