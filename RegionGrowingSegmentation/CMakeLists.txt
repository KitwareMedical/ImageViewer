# Include OpenGL
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)


INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_SOURCE_DIR}/Auxiliary/VtkFltk
${ITK_SOURCE_DIR}/Examples/RegionGrowingSegmentation
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Auxiliary/VtkFltk
${ITK_BINARY_DIR}/Examples/RegionGrowingSegmentation
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
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
vtkRendering
vtkGraphics	  
vtkHybrid	  
vtkImaging
vtkIO
vtkFiltering
vtkCommon
FltkImageViewer
vtkFltk
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

ADD_EXECUTABLE(RegionGrowingSegmentation2D ${RegionGrowingSegmentation2D_SRCS})
FLTK_WRAP_UI( RegionGrowingSegmentation2D ${RegionGrowingSegmentation2D_GUI_SRCS} )

ADD_EXECUTABLE(RegionGrowingSegmentation ${RegionGrowingSegmentation_SRCS})
FLTK_WRAP_UI( RegionGrowingSegmentation ${RegionGrowingSegmentation_GUI_SRCS} )

