# Add VTK code if we found VTK
INCLUDE (${CMAKE_ROOT}/Modules/FindVTK.cmake)
IF (USE_VTK_FILE)
  INCLUDE(${USE_VTK_FILE})
  LINK_LIBRARIES( 
      vtkRendering
      vtkGraphics	  
      vtkImaging
      vtkIO
      vtkFiltering
      vtkCommon
  )
ENDIF (USE_VTK_FILE)

LINK_LIBRARIES(
VXLNumerics
ITKCommon
)

ADD_EXECUTABLE(vtk2itk vtk2itk)

