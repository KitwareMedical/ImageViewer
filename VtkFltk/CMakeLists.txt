
# This directory only builds if FLTK and VTK can be found

SET(vtkFltk_SRCS
  vtkFlRenderWindowInteractor.cxx
  fltkVTKImageViewerBase.cxx
  )

INCLUDE_DIRECTORIES(
   ${FLTK_INCLUDE_PATH}
   ${VTK_INCLUDE_PATH}
   )

SET(fltkVTKImageViewer_GUI_SRCS
  fltkVTKImageViewerGUI.fl
)
 

FLTK_WRAP_UI(vtkFltk ${fltkVTKImageViewer_GUI_SRCS} )

ADD_LIBRARY( vtkFltk ${vtkFltk_SRCS} )


