PROJECT( ShapeDetectionLevelSet )

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_SOURCE_DIR}/Auxiliary/VtkFltk
${InsightApplications_SOURCE_DIR}/Auxiliary/vtk
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/VtkFltk
${ShapeDetectionLevelSet_SOURCE_DIR}
${ShapeDetectionLevelSet_BINARY_DIR}
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)


SET(ShapeDetectionLevelSet_SRCS
ShapeDetectionLevelSetBase.cxx
ShapeDetectionLevelSet.cxx
)

SET(ShapeDetectionLevelSet_GUI_SRCS
ShapeDetectionLevelSetGUI.fl
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

IF(WIN32)
  LINK_LIBRARIES(
  wsock32
  )
ENDIF(WIN32)

IF(UNIX)
  LINK_LIBRARIES (
  ${CMAKE_X_LIBS}
  )
ENDIF(UNIX)

ADD_EXECUTABLE(ShapeDetectionLevelSet ${ShapeDetectionLevelSet_SRCS})
FLTK_WRAP_UI( ShapeDetectionLevelSet ${ShapeDetectionLevelSet_GUI_SRCS} )

