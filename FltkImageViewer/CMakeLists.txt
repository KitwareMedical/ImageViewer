
# Only build this if FLTK can be found
IF(HAS_FLTK)

   SOURCE_FILES(FltkImageViewer_SRCS
     fltkBox3D
     fltkCone3D
     fltkClippingPlane3DDrawer
     fltkCylinder3D
     fltkFrustum3D
     fltkGlWindow
     fltkGlWindowInteractive
     fltkGlWindowInteractiveFlat
     fltkImage2DViewerWindow
     fltkRegularStepGradientDescent
     fltkShape3D
     fltkSphere3D
     fltkWindowInteractive
     fltkTimeProbeGUI
     fltkTimeProbesCollector
     )

  SOURCE_FILES(FltkImageViewer_GUI_SRCS
     fltkClippingPlane3DDrawerGUI.fl
     fltkDisplayGlWindowGUI.fl
     fltkDisplayGlWindowFlatGUI.fl
     fltkFrustumFunctionControlGUI.fl
     fltkImageViewerGUI.fl
     fltkImage2DViewerGUI.fl
     fltkRegularStepGradientDescentGUI.fl
     fltkSlice2DDrawerGUI.fl
     fltkSlice3DDrawerGUI.fl
     fltkSphereFunctionControlGUI.fl
     fltkTimeProbesCollectorGUI.fl
    )

  INCLUDE_DIRECTORIES(
     ${ITK_SOURCE_DIR}/Auxiliary/MetaImage/IView
     ${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
     ${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
     ${FLTK_INCLUDE_PATH}
     )

  ADD_LIBRARY(
     FltkImageViewer FltkImageViewer_SRCS
     )

#
# Take the list of GUI files, generate .cxx and .h from 
# them and add them to the build process of the target
#
  FLTK_WRAP_UI(FltkImageViewer FltkImageViewer_GUI_SRCS )

ENDIF(HAS_FLTK)

