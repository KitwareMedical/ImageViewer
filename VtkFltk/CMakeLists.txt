# This directory only builds if FLTK and VTK can be found
INCLUDE_DIRECTORIES(
  ${ITK_SOURCE_DIR}/Auxiliary/VtkFltk
  ${FLTK_INCLUDE_PATH}
  ${VTK_INCLUDE_PATH}
)

ADD_LIBRARY(vtkFltk
  vtkFlRenderWindowInteractor.cxx
  fltkVTKImageViewerBase.cxx
)

FLTK_WRAP_UI(vtkFltk fltkVTKImageViewerGUI.fl)

TARGET_LINK_LIBRARIES(vtkFltk ${FLTK_LIBRARIES} ${GLU_LIBRARY})

INSTALL_TARGETS(/lib/InsightToolkit vtkFltk)

INSTALL_FILES(/include/InsightToolkit/Auxiliary/FltkImageViewer ".h"
  fltkVTKImageViewerGUI
  )
