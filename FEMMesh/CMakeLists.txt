
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/Numerics
${ITK_SOURCE_DIR}/Code/Numerics/FEM
${ITK_SOURCE_DIR}/Code/Numerics/vxl
${ITK_SOURCE_DIR}/Code/Numerics/vxl/vcl
${ITK_SOURCE_DIR}/Examples/FEMMesh
${ITK_SOURCE_DIR}/Auxiliary/VtkFltk
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/Numerics
${ITK_BINARY_DIR}/Code/Numerics/FEM
${FLTK_LIB_PATH}
)

# Use VTK for this directory.  This is safe because we wouldn't be
# here if we didn't find VTK.
INCLUDE(${USE_VTK_FILE})
LINK_LIBRARIES( 
  vtkRendering
  vtkGraphics	  
  vtkImaging
  vtkIO
  vtkFiltering
  vtkCommon
  vtkFltk
)

SOURCE_FILES(FEMMeshApplication_SRCS
itkFEMHeatConduction2D
FEMMeshApplicationBase
FEMMeshApplication
main
)

SOURCE_FILES(FEMMeshApplication_GUI_SRCS
FEMMeshApplicationGUI.fl
)

LINK_LIBRARIES (
ITKCommon
ITKBasicFilters 
VXLNumerics
FEM
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

ADD_EXECUTABLE(FEMMesh FEMMeshApplication_SRCS)

FLTK_WRAP_UI(FEMMesh FEMMeshApplication_GUI_SRCS)


