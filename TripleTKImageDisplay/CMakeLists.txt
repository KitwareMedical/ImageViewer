INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Numerics/vxl
${ITK_SOURCE_DIR}/Code/Numerics/vxl/vcl
${ITK_SOURCE_DIR}/Auxiliary/MetaImage
${ITK_SOURCE_DIR}/Auxiliary/MetaImage/IView
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Auxiliary/MetaImage
${ITK_BINARY_DIR}/Auxiliary/MetaImage/IView
${FLTK_LIB_PATH}
)

# Add VTK code if we found VTK
INCLUDE (${CMAKE_ROOT}/Modules/FindVTK.cmake)
IF (USE_VTK_FILE)
  INCLUDE(${USE_VTK_FILE})
  LINK_LIBRARIES( 
      vtkRendering
      vtkGraphics	  
      vtkImaging
      vtkIO
      vtkFiltering
      vtkCommon
  )
ENDIF (USE_VTK_FILE)


SOURCE_FILES(TripleTKID_SRCS
3D/FLVTKWin
TripleTKMain
TripleTK2D
TripleTK3D
TripleTKGUI
TripleTKGUI_CB
)

LINK_LIBRARIES (
ITKCommon
ITKBasicFilters 
VXLNumerics
MetaImageIO
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

ADD_EXECUTABLE(TripleTKID TripleTKID_SRCS)

