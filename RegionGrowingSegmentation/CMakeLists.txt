PROJECT( RegionGrowingSegmentation )

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_SOURCE_DIR}/Auxiliary/VtkFltk
${InsightApplications_SOURCE_DIR}/Auxiliary/vtk
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/VtkFltk
${RegionGrowingSegmentation_SOURCE_DIR}
${RegionGrowingSegmentation_BINARY_DIR}
)

SET(RegionGrowingSegmentation2D_SRCS
RegionGrowingSegmentationBase2D.cxx
RegionGrowingSegmentation2D.cxx
)

SET(RegionGrowingSegmentation2D_GUI_SRCS
RegionGrowingSegmentation2DGUI.fl
)


SET(RegionGrowingSegmentation_SRCS
RegionGrowingSegmentationBase.cxx
RegionGrowingSegmentation.cxx
)

SET(RegionGrowingSegmentation_GUI_SRCS
RegionGrowingSegmentationGUI.fl
)

LINK_LIBRARIES (
ITKCommon
ITKNumerics
ITKBasicFilters 
ITKIO
ITKFltkImageViewer
ITKVtkFltk
vtkRendering
vtkGraphics	  
vtkHybrid	  
vtkImaging
vtkIO
vtkFiltering
vtkCommon
)


ADD_EXECUTABLE(RegionGrowingSegmentation2D ${RegionGrowingSegmentation2D_SRCS})
FLTK_WRAP_UI( RegionGrowingSegmentation2D ${RegionGrowingSegmentation2D_GUI_SRCS} )

ADD_EXECUTABLE(RegionGrowingSegmentation ${RegionGrowingSegmentation_SRCS})
FLTK_WRAP_UI( RegionGrowingSegmentation ${RegionGrowingSegmentation_GUI_SRCS} )

