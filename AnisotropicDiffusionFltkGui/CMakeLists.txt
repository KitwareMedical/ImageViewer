PROJECT( AnisotropicDiffusionFltkGui )

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/IO
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${AnisotropicDiffusionFltkGui_SOURCE_DIR}
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Code/IO
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${AnisotropicDiffusionFltkGui_BINARY_DIR}
)

SET(AnisotropicDiffusionFltkGui3D_SRCS
liFilterConsole3D.cxx
liFilterConsole3DBase.cxx
Filters3D.cxx
)

SET(AnisotropicDiffusionFltkGui2D_SRCS
liFilterConsole2D.cxx
liFilterConsole2DBase.cxx
Filters2D.cxx
)

SET(AnisotropicDiffusionFltkGui3D_GUI_SRCS
liFilterConsole3DGUI.fl
)

SET(AnisotropicDiffusionFltkGui2D_GUI_SRCS
liFilterConsole2DGUI.fl
)

LINK_LIBRARIES (
ITKBasicFilters 
ITKIO
ITKFltkImageViewer
${FLTK_LIBRARY}
${OPENGL_LIBRARY}
${GLU_LIBRARY}
)

IF(WIN32)
  LINK_LIBRARIES(
  wsock32
  )
ENDIF(WIN32)

ADD_EXECUTABLE(AnisotropicDiffusionFltkGui3D ${AnisotropicDiffusionFltkGui3D_SRCS})
ADD_EXECUTABLE(AnisotropicDiffusionFltkGui2D ${AnisotropicDiffusionFltkGui2D_SRCS})

FLTK_WRAP_UI(AnisotropicDiffusionFltkGui3D   ${AnisotropicDiffusionFltkGui3D_GUI_SRCS})
FLTK_WRAP_UI(AnisotropicDiffusionFltkGui2D   ${AnisotropicDiffusionFltkGui2D_GUI_SRCS})



