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

LINK_LIBRARIES(VXLNumerics ITKCommon)

SOURCE_FILES(MRIRegistration_SRCS
OptionList
RegisterAD
vtkImageStatistics
)

ADD_EXECUTABLE(MRIRegistration MRIRegistration_SRCS)
