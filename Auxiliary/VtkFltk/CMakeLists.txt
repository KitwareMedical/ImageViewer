# This directory only builds if FLTK and VTK can be found
INCLUDE_DIRECTORIES(
  ${InsightApplications_SOURCE_DIR}/Auxiliary/VtkFltk
  ${InsightApplications_SOURCE_DIR}/Auxiliary/vtk
)

ADD_LIBRARY(ITKVtkFltk
  vtkFlRenderWindowInteractor.cxx
  fltkVTKImageViewerBase.cxx
)

FLTK_WRAP_UI(ITKVtkFltk fltkVTKImageViewerGUI.fl)

TARGET_LINK_LIBRARIES(ITKVtkFltk vtkRendering ${FLTK_LIBRARIES} ${GLU_LIBRARY})
