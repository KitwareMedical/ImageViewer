INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_SOURCE_DIR}/Auxiliary/VtkFltk
${ITK_SOURCE_DIR}/Auxiliary/vtk
${ITK_SOURCE_DIR}/Examples/ShapeDetectionLevelSet
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Auxiliary/VtkFltk
${ITK_BINARY_DIR}/Examples/ShapeDetectionLevelSet
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

