# Include OpenGL
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)


INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_SOURCE_DIR}/Auxiliary/VtkFltk
${ITK_SOURCE_DIR}/Examples/FastMarchingLevelSet
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Auxiliary/VtkFltk
${ITK_BINARY_DIR}/Examples/FastMarchingLevelSet
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
vtkRendering
vtkGraphics	  
vtkHybrid	  
vtkImaging
vtkIO
vtkFiltering
vtkCommon
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

ADD_EXECUTABLE(FastMarchingLevelSet ${FastMarchingLevelSet_SRCS})
FLTK_WRAP_UI( FastMarchingLevelSet ${FastMarchingLevelSet_GUI_SRCS} )

