PROJECT(Curves2DExtractor)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
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

IF(WIN32)
  TARGET_LINK_LIBRARIES(Curves2DExtractorLib wsock32)
ENDIF(WIN32)

IF(UNIX)
  TARGET_LINK_LIBRARIES(Curves2DExtractorLib ${CMAKE_X_LIBS})
ENDIF(UNIX)

ADD_EXECUTABLE(Curves2DExtractor main.cxx)
TARGET_LINK_LIBRARIES(Curves2DExtractor Curves2DExtractorLib)

