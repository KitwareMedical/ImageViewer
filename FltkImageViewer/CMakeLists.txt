INCLUDE(${CMAKE_SOURCE_DIR}/CMake/Modules/FindFLTK.cmake)

# Only build this if FLTK can be found
IF(FLTK_LIB_PATH)
  IF(FLTK_INCLUDE_PATH)

   SOURCE_FILES(FltkImageViewer_SRCS
     fltkDisplayGLWindowGUI
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

  ENDIF(FLTK_LIB_PATH)
ENDIF(FLTK_INCLUDE_PATH)

