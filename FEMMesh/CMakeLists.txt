
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Examples/FEMMesh
${ITK_SOURCE_DIR}/Auxiliary/VtkFltk
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

LINK_DIRECTORIES(
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

SET(FEMMeshApplication_SRCS
itkFEMHeatConduction2D.cxx
FEMMeshApplicationBase.cxx
FEMMeshApplication.cxx
main.cxx
)

SET(FEMMeshApplication_GUI_SRCS
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

ADD_EXECUTABLE(FEMMesh ${FEMMeshApplication_SRCS})

FLTK_WRAP_UI(FEMMesh ${FEMMeshApplication_GUI_SRCS})
