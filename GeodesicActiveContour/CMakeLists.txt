PROJECT( GeodesicActiveContour )

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_SOURCE_DIR}/Auxiliary/VtkFltk
${InsightApplications_SOURCE_DIR}/Auxiliary/vtk
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/VtkFltk
${GeodesicActiveContour_SOURCE_DIR}
${GeodesicActiveContour_BINARY_DIR}
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)


SET(GeodesicActiveContour_SRCS
GeodesicActiveContourBase.cxx
GeodesicActiveContour.cxx
)

SET(GeodesicActiveContour_GUI_SRCS
GeodesicActiveContourGUI.fl
)

LINK_LIBRARIES (
ITKCommon
ITKNumerics
ITKBasicFilters 
ITKIO
vtkRendering
vtkGraphics	  
vtkHybrid	  
vtkImaging
vtkIO
vtkFiltering
vtkCommon
ITKFltkImageViewer
ITKVtkFltk
${FLTK_LIBRARY}
${OPENGL_LIBRARY}
${GLU_LIBRARY}
)

ADD_EXECUTABLE(GeodesicActiveContour ${GeodesicActiveContour_SRCS})
FLTK_WRAP_UI( GeodesicActiveContour ${GeodesicActiveContour_GUI_SRCS} )

