FIND_INCLUDE( FLTK_INCLUDE_DIR FL/Fl.h H:/usr/local/fltk )

LIBRARY(
IView
)

SOURCE_FILES(
IView
IViewGUI
IViewGUIBase
)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Unsupported/MetaImage
${ITK_SOURCE_DIR}/Unsupported/MetaImage/IView
${FLTK_INCLUDE_DIR}
)


