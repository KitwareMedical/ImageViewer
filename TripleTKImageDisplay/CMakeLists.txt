INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Numerics/vxl
${ITK_SOURCE_DIR}/Code/Numerics/vxl/vcl
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
${GLU_INCLUDE_PATH}
${ITK_SOURCE_DIR}/Examples/TripleTKImageDisplay
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${FLTK_LIB_PATH}
)

# Use VTK for this directory.  This is safe because we wouldn't be
# here if we didn't find VTK.
INCLUDE(${USE_VTK_FILE})
LINK_LIBRARIES( 
  vtkRendering
  vtkGraphics	  
  vtkHybrid	  
  vtkImaging
  vtkIO
  vtkFiltering
  vtkCommon
  vtkFltk
)

SET(TripleTKID_SRCS
3D/FLVTKWin.cxx
TripleTKMain.cxx
TripleTK2D.cxx
TripleTK3D.cxx
TripleTKGUI_CB.cxx
)

SET(TripleTKID_GUI_SRCS
TripleTKGUI.fld
)

LINK_LIBRARIES (
ITKCommon
ITKBasicFilters 
VXLNumerics
FltkImageViewer
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
  -lm
  )
ENDIF(UNIX)

ADD_EXECUTABLE(TripleTKID ${TripleTKID_SRCS})
FLTK_WRAP_UI(TripleTKID     ${TripleTKID_GUI_SRCS})


