PROJECT( MutualInformationEuler2DRegistration )

INCLUDE_REGULAR_EXPRESSION(".*")

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${MutualInformationEuler2DRegistration_SOURCE_DIR}
${MutualInformationEuler2DRegistration_BINARY_DIR}
)

SET(MutualInformationEuler2DRegistration_SRCS
  RegisterApplication.cxx
  RegisterWindow.cxx
  register.cxx
)

ADD_GUI_EXECUTABLE(MutualInformationEuler2DRegistration
  "${MutualInformationEuler2DRegistration_SRCS}")

FLTK_WRAP_UI(MutualInformationEuler2DRegistration registerGUI.fl)

TARGET_LINK_LIBRARIES(MutualInformationEuler2DRegistration
                      ITKNumerics ITKBasicFilters ITKIO ITKFltkImageViewer)


