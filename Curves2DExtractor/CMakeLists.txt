PROJECT(Curves2DExtractor)

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${Curves2DExtractor_SOURCE_DIR}
${Curves2DExtractor_BINARY_DIR}
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

ADD_LIBRARY(Curves2DExtractorLib
            ceExtractorConsole.cxx
            ceExtractorConsoleBase.cxx)

#
# Take the list of GUI files, generate .cxx and .h from 
# them and add them to the build process of the target
#
FLTK_WRAP_UI(Curves2DExtractorLib ceExtractorConsoleGUI.fl)

TARGET_LINK_LIBRARIES (Curves2DExtractorLib
                       ITKBasicFilters ITKIO ITKFltkImageViewer
                       ${FLTK_LIBRARY} ${OPENGL_LIBRARY} ${GLU_LIBRARY})

ADD_EXECUTABLE(Curves2DExtractor main.cxx)
TARGET_LINK_LIBRARIES(Curves2DExtractor Curves2DExtractorLib)

