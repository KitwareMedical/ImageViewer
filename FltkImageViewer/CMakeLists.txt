INCLUDE (${CMAKE_ROOT}/Modules/FindFLTK.cmake)

# Only build this if FLTK can be found
IF(FLTK_LIBRARY)
  IF(FLTK_INCLUDE_PATH)

   SOURCE_FILES(FltkImageViewer_SRCS
     fltkBox3D
     fltkCone3D
     fltkCylinder3D
     fltkDisplayGlWindowGUI
     fltkDisplayGlWindowFlatGUI
     fltkGlWindow
     fltkGlWindowInteractive
     fltkGlWindowInteractiveFlat
     fltkImageViewerGUI
     fltkImage2DViewerGUI
     fltkImage2DViewerWindow
     fltkImage2DViewerUpdateCommand
     fltkLightButtonRedrawCommand
     fltkProgressBarRedrawCommand
     fltkRegularStepGradientDescent
     fltkRegularStepGradientDescentGUI
     fltkSlice3DDrawerGUI
     fltkShape3D
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

