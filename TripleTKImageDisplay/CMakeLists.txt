INCLUDE_DIRECTORIES(
  ${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_SOURCE_DIR}/TripleTKImageDisplay
)

ADD_EXECUTABLE(TripleTKID 
  3D/FLVTKWin.cxx
  TripleTKMain.cxx
  TripleTK2D.cxx
  TripleTK3D.cxx
  TripleTKGUI_CB.cxx
)
FLTK_WRAP_UI(TripleTKID TripleTKGUI.fld)

TARGET_LINK_LIBRARIES (TripleTKID
                       ITKIO ITKBasicFilters ITKFltkImageViewer ITKVtkFltk
                       vtkHybrid vtkIO)
