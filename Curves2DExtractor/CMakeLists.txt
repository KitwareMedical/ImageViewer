INCLUDE_DIRECTORIES(
  ${InsightApplications_SOURCE_DIR}/Curves2DExtractor
  ${InsightApplications_BINARY_DIR}/Curves2DExtractor
  ${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
  ${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
)

ADD_LIBRARY(Curves2DExtractorLib ceExtractorConsole.cxx
            ceExtractorConsoleBase.cxx)

# Take the list of GUI files, generate .cxx and .h from them and add
# them to the build process of the target
FLTK_WRAP_UI(Curves2DExtractorLib ceExtractorConsoleGUI.fl)

TARGET_LINK_LIBRARIES(Curves2DExtractorLib
                      ITKBasicFilters ITKIO ITKFltkImageViewer)

ADD_EXECUTABLE(Curves2DExtractor main.cxx)
TARGET_LINK_LIBRARIES(Curves2DExtractor Curves2DExtractorLib)
