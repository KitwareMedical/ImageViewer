PROJECT(MatroxInput)

# 
# Find ITK
#
INCLUDE (${CMAKE_ROOT}/Modules/FindITK.cmake)
IF (USE_ITK_FILE)
  INCLUDE(${USE_ITK_FILE})
ENDIF(USE_ITK_FILE)

#
# Include FLTK configuration
#
INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)


#
# Include VTK configuration
#
INCLUDE (${CMAKE_ROOT}/Modules/FindVTK.cmake)
IF (USE_VTK_FILE)
  INCLUDE (${USE_VTK_FILE})
ENDIF (USE_VTK_FILE)



INCLUDE_DIRECTORIES (
  ${MatroxInput_SOURCE_DIR}
  ${ITK_SOURCE_DIR}
  ${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
  ${ITK_SOURCE_DIR}/Auxiliary/VtkFltk
  ${FLTK_INCLUDE_DIR}
  ${VTK_INCLUDE_DIR}
)

LINK_DIRECTORIES(
  ${ITK_LIBRARY_PATH}  
  ${VTK_LIBRARY_PATH}  
)


LINK_LIBRARIES (
  FltkImageViewer
  vtkFltk
  vtkRendering
  vtkGraphics	  
  vtkHybrid	  
  vtkImaging
  vtkIO
  vtkFiltering
  vtkCommon
  ITKCommon
  VXLNumerics
  ${FLTK_LIBRARY}
  ${OPENGL_LIBRARY}
  ${GLU_LIBRARY}
)

SET(MatroxInput_SRCS
  MatroxViewerBase.cxx
  MatroxViewer.cxx
  main.cxx
)

SET(MatroxInput_GUI_SRCS
  MatroxViewerGUI.fd
)


ADD_EXECUTABLE(MatroxInput MatroxInput_SRCS)


FLTK_WRAP_UI(MatroxInput ${MatroxInput_GUI_SRCS})


