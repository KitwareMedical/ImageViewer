
SOURCE_FILES(IView_SRCS
  IView
)

SOURCE_FILES(IView_GUI_SRCS
  IViewGUIBase.fld
  IViewGUI.fld
)


INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Auxiliary/MetaImage
${ITK_SOURCE_DIR}/Auxiliary/MetaImage/IView
${ITK_BINARY_DIR}/Auxiliary/MetaImage/IView
${FLTK_INCLUDE_PATH}
)

ADD_LIBRARY(
IView IView_SRCS
)

#
# Take the list of GUI files, generate .cxx and .h from 
# them and add them to the build process of the target
#
FLTK_WRAP_UI(IView IView_GUI_SRCS)



