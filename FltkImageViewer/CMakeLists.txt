
# Only build this if FLTK can be found
IF(HAS_FLTK)

     SET(FltkImageViewer_SRCS
     fltkBox3D.cxx
     fltkCone3D.cxx
     fltkClippingPlane3DDrawer.cxx
     fltkCylinder3D.cxx
     fltkFrustum3D.cxx
     fltkGlWindow.cxx
     fltkGlWindowInteractive.cxx
     fltkGlWindowInteractiveFlat.cxx
     fltkImage2DViewerWindow.cxx
     fltkRGBImage2DViewerWindow.cxx
     fltkLightButton.cxx
     fltkProgressBar.cxx
     fltkShape3D.cxx
     fltkSphere3D.cxx
     fltkWindowInteractive.cxx
     fltkTimeProbeGUI.cxx
     fltkTimeProbesCollector.cxx
     fltkUtils.cxx
     )

  SET(FltkImageViewer_GUI_SRCS
     fltkClippingPlane3DDrawerGUI.fl
     fltkDisplayGlWindowGUI.fl
     fltkDisplayGlWindowFlatGUI.fl
     fltkFrustumFunctionControlGUI.fl
     fltkImageViewerGUI.fl
     fltkImage2DViewerGUI.fl
     fltkRGBImage2DViewerGUI.fl
     fltkRegularStepGradientDescentGUI.fl
     fltkSlice2DDrawerGUI.fl
     fltkSlice3DDrawerGUI.fl
     fltkSphereFunctionControlGUI.fl
     fltkTimeProbesCollectorGUI.fl
    )

  INCLUDE_DIRECTORIES(
     ${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
     ${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
     ${FLTK_INCLUDE_PATH}
     )

  ADD_LIBRARY(
     FltkImageViewer ${FltkImageViewer_SRCS}
     )

#
# Take the list of GUI files, generate .cxx and .h from 
# them and add them to the build process of the target
#
  FLTK_WRAP_UI(FltkImageViewer ${FltkImageViewer_GUI_SRCS} )

ENDIF(HAS_FLTK)


