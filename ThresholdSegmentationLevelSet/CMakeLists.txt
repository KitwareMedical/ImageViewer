PROJECT( ThresholdSegmentationLevelSet )

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_SOURCE_DIR}/Auxiliary/VtkFltk
${InsightApplications_SOURCE_DIR}/Auxiliary/vtk
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/VtkFltk
${ThresholdSegmentationLevelSet_SOURCE_DIR}
${ThresholdSegmentationLevelSet_BINARY_DIR}
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)


SET(ThresholdSegmentationLevelSet_SRCS
ThresholdSegmentationLevelSetBase.cxx
ThresholdSegmentationLevelSet.cxx
)

SET(ThresholdSegmentationLevelSet_GUI_SRCS
ThresholdSegmentationLevelSetGUI.fl
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
${FLTK_LIBRARY}
${OPENGL_LIBRARY}
${GLU_LIBRARY}
)


ADD_EXECUTABLE(ThresholdSegmentationLevelSet ${ThresholdSegmentationLevelSet_SRCS})
FLTK_WRAP_UI( ThresholdSegmentationLevelSet ${ThresholdSegmentationLevelSet_GUI_SRCS} )

