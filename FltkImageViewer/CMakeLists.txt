FIND_INCLUDE( FLTK_INCLUDE_DIR FL/Fl.H H:/usr/local/fltk )

LIBRARY(
FltkImageViewer
)

SOURCE_FILES(
fltkImageViewerGUI 
fltkSlice3DDrawerGUI
)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Unsupported/MetaImage/IView
${ITK_SOURCE_DIR}/Unsupported/FltkImageViewer
${FLTK_INCLUDE_DIR}
)


