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

INCLUDE_DIRECTORIES(${ITK_SOURCE_DIR}/Code/BasicFilters)
INCLUDE_DIRECTORIES(${ITK_SOURCE_DIR}/Code/Algorithms)

LINK_LIBRARIES(
VXLNumerics
ITKCommon
ITKBasicFilters
)

ADD_EXECUTABLE(vtk2itk vtk2itk)
ADD_EXECUTABLE(itkCurvatureFlowToVTK itkCurvatureFlowToVTK)
ADD_EXECUTABLE(itkVTKtoITKtoVTK itkVTKtoITKtoVTK)
