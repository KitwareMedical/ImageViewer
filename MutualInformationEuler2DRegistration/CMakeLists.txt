PROJECT( MutualInformationEuler2DRegistration )

INCLUDE_REGULAR_EXPRESSION(".*")

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${MutualInformationEuler2DRegistration_SOURCE_DIR}
${MutualInformationEuler2DRegistration_BINARY_DIR}
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

ADD_EXECUTABLE(MutualInformationEuler2DRegistration
  RegisterApplication.cxx
  RegisterWindow.cxx
  register.cxx
)

FLTK_WRAP_UI(MutualInformationEuler2DRegistration registerGUI.fl)

TARGET_LINK_LIBRARIES(MutualInformationEuler2DRegistration
                      ITKNumerics ITKBasicFilters ITKIO ITKFltkImageViewer
                      ${FLTK_LIBRARY} ${OPENGL_LIBRARY} ${GLU_LIBRARY})


