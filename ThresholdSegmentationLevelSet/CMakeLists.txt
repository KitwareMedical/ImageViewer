# Include OpenGL
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)


INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_SOURCE_DIR}/Auxiliary/VtkFltk
${ITK_SOURCE_DIR}/Auxiliary/vtk
${ITK_SOURCE_DIR}/Examples/ThresholdSegmentationLevelSet
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Auxiliary/VtkFltk
${ITK_BINARY_DIR}/Examples/ThresholdSegmentationLevelSet
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

ADD_EXECUTABLE(ThresholdSegmentationLevelSet ${ThresholdSegmentationLevelSet_SRCS})
FLTK_WRAP_UI( ThresholdSegmentationLevelSet ${ThresholdSegmentationLevelSet_GUI_SRCS} )

