
# This directory only builds if FLTK and VTK can be found

SET(vtkFltk_SRCS
  vtkFlRenderWindowInteractor.cxx
   )

INCLUDE_DIRECTORIES(
   ${FLTK_INCLUDE_PATH}
   ${VTK_INCLUDE_PATH}
   )

ADD_LIBRARY( vtkFltk ${vtkFltk_SRCS} )


