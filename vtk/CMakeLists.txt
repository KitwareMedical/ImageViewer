# Add VTK code if we found VTK
IF (HAS_VTK)
  INCLUDE(${USE_VTK_FILE})
  LINK_LIBRARIES( 
      vtkRendering
      vtkGraphics	  
      vtkImaging
      vtkIO
      vtkFiltering
      vtkCommon
  )
ENDIF (HAS_VTK)

INCLUDE_DIRECTORIES(${ITK_SOURCE_DIR}/Code/BasicFilters)
INCLUDE_DIRECTORIES(${ITK_SOURCE_DIR}/Code/Algorithms)

LINK_LIBRARIES(
VXLNumerics
ITKCommon
ITKBasicFilters
vtkpng
vtkzlib
)

ADD_EXECUTABLE(vtk2itk vtk2itk.cxx)
ADD_EXECUTABLE(itkCurvatureFlowToVTK itkCurvatureFlowToVTK.cxx)
ADD_EXECUTABLE(itkVTKtoITKtoVTK itkVTKtoITKtoVTK.cxx)
ADD_EXECUTABLE(itkPNGVTKtoITKtoVTK itkPNGVTKtoITKtoVTK.cxx)
