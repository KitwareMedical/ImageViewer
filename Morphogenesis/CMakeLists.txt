INCLUDE_DIRECTORIES(
  ${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_SOURCE_DIR}/Morphogenesis
  ${InsightApplications_BINARY_DIR}/Morphogenesis
)

ADD_EXECUTABLE(Morphogenesis
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

FLTK_WRAP_UI(Morphogenesis BacterialColonyGUI.fl CellsViewerGUI.fl)
TARGET_LINK_LIBRARIES(Morphogenesis ITKFltkImageViewer ITKBasicFilters ITKIO)
