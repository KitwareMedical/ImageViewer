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
)


