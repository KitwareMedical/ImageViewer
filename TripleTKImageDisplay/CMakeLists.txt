
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
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${FLTK_LIB_PATH}
)

# Use VTK for this directory.  This is safe because we wouldn't be
# here if we didn't find VTK.
INCLUDE(${USE_VTK_FILE})
ADD_EXECUTABLE(TripleTKID 
  3D/FLVTKWin.cxx
  TripleTKMain.cxx
  TripleTK2D.cxx
  TripleTK3D.cxx
  TripleTKGUI_CB.cxx
)
FLTK_WRAP_UI(TripleTKID TripleTKGUI.fld)

TARGET_LINK_LIBRARIES (TripleTKID
                       ITKIO ITKBasicFilters ITKFltkImageViewer ITKVtkFltk
                       ${FLTK_LIBRARY} ${OPENGL_LIBRARY} ${GLU_LIBRARY})

TARGET_LINK_LIBRARIES(TripleTKID
  vtkRendering
  vtkGraphics	  
  vtkHybrid	  
  vtkImaging
  vtkIO
  vtkFiltering
  vtkCommon
)

IF(WIN32)
  TARGET_LINK_LIBRARIES(TripleTKID wsock32)
ENDIF(WIN32)

IF(UNIX)
  TARGET_LINK_LIBRARIES(TripleTKID -lm)
ENDIF(UNIX)
