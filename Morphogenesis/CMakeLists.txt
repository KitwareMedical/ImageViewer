INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Utilities/zlib
${ITK_SOURCE_DIR}/Utilities/png
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/Morphogenesis
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Examples/Morphogenesis
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
${GLU_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Code/IO
${ITK_BINARY_DIR}/Utilities/png
${ITK_BINARY_DIR}/Utilities/zlib
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Auxiliary/MetaImage/IView
${ITK_BINARY_DIR}/Examples/Morphogenesis
)

SOURCE_FILES(Morphogenesis_SRCS
Bacteria
BacterialColony
Cell
CellsViewerBase
CellsViewer
CellsViewerCommand
CellularAggregate
Eukariote
GradientEatingBacteria
Genome
PressureSensitiveBacteria
Prokariote
main
)

SOURCE_FILES(Morphogenesis_GUI_SRCS
BacterialColonyGUI.fl
CellsViewerGUI.fl
)

LINK_LIBRARIES (
ITKCommon
ITKBasicFilters 
ITKIO
IView
itkpng
itkzlib
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
  ${CMAKE_X_LIBS} 
  -lm
  )
ENDIF(UNIX)

ADD_EXECUTABLE(Morphogenesis Morphogenesis_SRCS)

FLTK_WRAP_UI(Morphogenesis  Morphogenesis_GUI_SRCS)

