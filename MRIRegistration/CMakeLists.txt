# Use VTK for this directory.  This is safe because we wouldn't be
# here if we didn't find VTK.

INCLUDE(${USE_VTK_FILE})
LINK_LIBRARIES( 
  vtkRendering
  vtkGraphics	  
  vtkImaging
  vtkIO
  vtkFiltering
  vtkCommon
)

LINK_LIBRARIES(VXLNumerics ITKCommon ITKIO ITKNumerics)

SET(MIRegistration_SRCS
OptionList
MIRegistration
)

ADD_EXECUTABLE(MIRegistration ${MIRegistration_SRCS})
