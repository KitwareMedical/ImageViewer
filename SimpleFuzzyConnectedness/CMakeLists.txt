PROJECT (SimpleFuzzyConnectedness)

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${SimpleFuzzyConnectedness_SOURCE_DIR}
)

LINK_LIBRARIES (
ITKCommon
ITKNumerics
ITKBasicFilters 
ITKIO
ITKFltkImageViewer

)

SET(Project_SRCS
FuzzySegMain.cpp
FuzzySegGrayRun.cxx
)

SET(Project_FLTK_SRCS
FuzzySegGrayUI.fl
)

ADD_GUI_EXECUTABLE(SimpleFuzzyConnectedness   "${Project_SRCS}")

FLTK_WRAP_UI(SimpleFuzzyConnectedness ${Project_FLTK_SRCS})

