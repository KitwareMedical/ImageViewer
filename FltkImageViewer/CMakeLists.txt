FIND_INCLUDE( FLTK_INCLUDE_DIR FL/Fl.H H:/usr/local/fltk )

LIBRARY(
FltkImageViewer
)

SOURCE_FILES(
fltkGlWindow
fltkGlWindowInter
fltkGlWindowInterFlat
fltkImageViewerGUI
fltkLightButtonRedrawCommand
fltkProgressBarRedrawCommand
fltkSlice3DDrawerGUI
fltkWindowInter
)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Unsupported/MetaImage/IView
${ITK_SOURCE_DIR}/Unsupported/FltkImageViewer
${FLTK_INCLUDE_DIR}
)


