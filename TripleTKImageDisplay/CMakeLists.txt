INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindVTK.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Numerics/vxl
${ITK_SOURCE_DIR}/Code/Numerics/vxl/vcl
${ITK_SOURCE_DIR}/Unsupported/MetaImage
${ITK_SOURCE_DIR}/Unsupported/MetaImage/IView
${FLTK_INCLUDE_PATH}
${VTK_INCLUDE_PATH}
${VTK_INCLUDE_PATH}/common
${VTK_INCLUDE_PATH}/graphics
${VTK_INCLUDE_PATH}/vtkbin
${OPENGL_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Unsupported/MetaImage
${ITK_BINARY_DIR}/Unsupported/MetaImage/IView
${FLTK_LIB_PATH}
${VTK_LIB_PATH}
)

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
${VTK_LIBRARY}
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

