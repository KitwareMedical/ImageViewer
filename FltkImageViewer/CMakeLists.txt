INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)

# Only build this if FLTK can be found
IF(FLTK_LIBRARY)
  IF(FLTK_INCLUDE_PATH)

   SOURCE_FILES(FltkImageViewer_SRCS
     fltkDisplayGlWindowGUI
     fltkGlWindow
     fltkGlWindowInteractive
     fltkGlWindowInteractiveFlat
     fltkImageViewerGUI
     fltkLightButtonRedrawCommand
     fltkProgressBarRedrawCommand
     fltkRegularStepGradientDescent
     fltkRegularStepGradientDescentGUI
     fltkSlice3DDrawerGUI
     fltkWindowInteractive
     )

  INCLUDE_DIRECTORIES(
     ${ITK_SOURCE_DIR}/Unsupported/MetaImage/IView
     ${ITK_SOURCE_DIR}/Unsupported/FltkImageViewer
     ${FLTK_INCLUDE_PATH}
     )

  ADD_LIBRARY(
     FltkImageViewer FltkImageViewer_SRCS
     )

  ENDIF(FLTK_LIBRARY)
ENDIF(FLTK_INCLUDE_PATH)

