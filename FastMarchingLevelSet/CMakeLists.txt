PROJECT(FastMarchingLevelSet)

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_SOURCE_DIR}/Auxiliary/VtkFltk
${InsightApplications_SOURCE_DIR}/Auxiliary/vtk
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/VtkFltk
${FastMarchingLevelSet_SOURCE_DIR}
${FastMarchingLevelSet_BINARY_DIR}
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)


SET(FastMarchingLevelSet_SRCS
FastMarchingLevelSetBase.cxx
FastMarchingLevelSet.cxx
)

SET(FastMarchingLevelSet_GUI_SRCS
FastMarchingLevelSetGUI.fl
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


ADD_EXECUTABLE(FastMarchingLevelSet ${FastMarchingLevelSet_SRCS})
FLTK_WRAP_UI( FastMarchingLevelSet ${FastMarchingLevelSet_GUI_SRCS} )

