
# Look for OpenGL
INCLUDE (${ITK_SOURCE_DIR}/Utilities/ITKFindOpenGL.cmake)
  
INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/Morphogenesis
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Examples/Morphogenesis
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
${GLU_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Examples/Morphogenesis
)

SET(Morphogenesis_SRCS
Bacteria.cxx
BacterialColony.cxx
Cell.cxx
CellsViewerBase.cxx
CellsViewer.cxx
CellsViewerCommand.cxx
CellularAggregate.cxx
Eukariote.cxx
GradientEatingBacteria.cxx
BilayerOrganism.cxx
Genome.cxx
PressureSensitiveBacteria.cxx
Prokariote.cxx
main.cxx
)

SET(Morphogenesis_GUI_SRCS
BacterialColonyGUI.fl
CellsViewerGUI.fl
)

LINK_LIBRARIES (
ITKBasicFilters 
ITKIO
ITKFltkImageViewer
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
  -lm
  )
ENDIF(UNIX)

ADD_EXECUTABLE(Morphogenesis ${Morphogenesis_SRCS})

FLTK_WRAP_UI(Morphogenesis  ${Morphogenesis_GUI_SRCS})

