FIND_PATH(VTK_32_PATH common/vtkConfigure.h.in c:/vtk)
FIND_PATH(VTK_40_PATH vtkCMakeOptions.cmake c:/vtknew)
FIND_PATH(VTK_BIN_PATH vtkConfigure.h c:/vtkbin /usr/local/vtk /vtknew-vc )
FIND_LIBRARY(VTK_LIBRARY vtkdll vtk c:/vtkbin/vtkdll /vtkbin/vtkdll /usr/local/vtk )


IF(WIN32)
LINK_LIBRARIES(opengl32)
ENDIF(WIN32)

IF(VTK_40_PATH)
  INCLUDE_DIRECTORIES(
  ${VTK_40_PATH}/Common
  ${VTK_40_PATH}/Filtering
  ${VTK_40_PATH}/Graphics
  ${VTK_40_PATH}/Hybrid
  ${VTK_40_PATH}/IO
  ${VTK_40_PATH}/Imaging
  ${VTK_40_PATH}/Parallel
  ${VTK_40_PATH}/Patented
  ${VTK_40_PATH}/Rendering
  ${VTK_BIN_PATH}
  ) 
  LINK_LIBRARIES(
   vtkCommon     vtkGraphics  vtkImaging
   vtkFiltering  vtkIO        vtkRendering 
  )
ELSE(VTK_40_PATH)
 INCLUDE_DIRECTORIES(
  ${VTK_INCLUDE_PATH}/common
  ${VTK_INCLUDE_PATH}/graphics
  ${VTK_INCLUDE_PATH}/imaging
  ${VTK_INCLUDE_PATH}/contrib
  ${VTK_BIN_PATH}
  ) 
  LINK_LIBRARIES(${VTK_LIBRARY})
ENDIF(VTK_40_PATH)

LINK_DIRECTORIES(${VTK_BIN_PATH}/lib)

LINK_LIBRARIES(
${VTK_LIBRARY}
VXLNumerics
ITKCommon
)

ADD_EXECUTABLE(vtk2itk vtk2itk)

