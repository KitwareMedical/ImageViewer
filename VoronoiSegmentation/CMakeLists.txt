PROJECT (VoronoiSegmentation)

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${VoronoiSegmentation_SOURCE_DIR}
)

LINK_LIBRARIES (
ITKCommon
ITKNumerics
ITKBasicFilters 
ITKIO
ITKFltkImageViewer

)


SET(Project_SRCS
VDSegMain.cpp
VDSegGrayRun.cxx
)

SET(Project_FLTK_SRCS
VDSegGrayUI.fl
)

ADD_GUI_EXECUTABLE(VoronoiSegmentation   "${Project_SRCS}")

FLTK_WRAP_UI(VoronoiSegmentation ${Project_FLTK_SRCS})

