FIND_INCLUDE( FLTK_INCLUDE_DIR FL/Fl.h H:/usr/local/fltk /usr/local/fltk )

SOURCE_FILES(IView_SRCS
IView
IViewGUI
IViewGUIBase
)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Unsupported/MetaImage
${ITK_SOURCE_DIR}/Unsupported/MetaImage/IView
${FLTK_INCLUDE_DIR}
)

ADD_LIBRARY(
IView IView_SRCS
)


