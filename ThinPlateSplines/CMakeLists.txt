
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/Numerics
${ITK_SOURCE_DIR}/Code/Numerics/vxl
${ITK_SOURCE_DIR}/Code/Numerics/vxl/vcl
${ITK_SOURCE_DIR}/Examples/ThinPlateSplines
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Examples/ThinPlateSplines
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/Numerics
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
  FltkImageViewer
)

SOURCE_FILES(ThinPlateSplinesApplication_SRCS
ThinPlateSplinesApplicationBase
ThinPlateSplinesApplication
vtkFlRenderWindowInteractor
main
)

SOURCE_FILES(ThinPlateSplinesApplication_GUI_SRCS
ThinPlateSplinesApplicationGUI.fl
)

LINK_LIBRARIES (
ITKCommon
ITKBasicFilters 
VXLNumerics
ITKIO
${FLTK_LIBRARY}
${OPENGL_LIBRARY}
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

ADD_EXECUTABLE(ThinPlateSplines ThinPlateSplinesApplication_SRCS)

FLTK_WRAP_UI(ThinPlateSplines ThinPlateSplinesApplication_GUI_SRCS)


