# This directory only builds if FLTK and VTK can be found
INCLUDE_DIRECTORIES(
  ${ITK_SOURCE_DIR}/Auxiliary/VtkFltk
  ${ITK_SOURCE_DIR}/Auxiliary/vtk
  ${FLTK_INCLUDE_PATH}
  ${VTK_INCLUDE_PATH}
)

ADD_LIBRARY(ITKVtkFltk
  vtkFlRenderWindowInteractor.cxx
  fltkVTKImageViewerBase.cxx
)

FLTK_WRAP_UI(ITKVtkFltk fltkVTKImageViewerGUI.fl)

TARGET_LINK_LIBRARIES(ITKVtkFltk ${FLTK_LIBRARIES} ${GLU_LIBRARY})
