INCLUDE_DIRECTORIES(${ITK_SOURCE_DIR}/Code/BasicFilters)
INCLUDE_DIRECTORIES(${ITK_SOURCE_DIR}/Code/Algorithms)

# Add VTK code if we found VTK
IF (HAS_VTK)
  INCLUDE(${USE_VTK_FILE})

  FOREACH(exe vtk2itk vtk2itkDouble itkCurvatureFlowToVTK
              itkVTKtoITKtoVTK itkPNGVTKtoITKtoVTK)
    ADD_EXECUTABLE(${exe} ${exe}.cxx)
    TARGET_LINK_LIBRARIES(${exe} ITKBasicFilters vtkRendering)
  ENDFOREACH(exe)
ENDIF (HAS_VTK)
