PROJECT( GradientRecursiveGaussianFilter )

INCLUDE_DIRECTORIES(
${InsightApplications_SOURCE_DIR}/Auxiliary/FltkImageViewer
${InsightApplications_BINARY_DIR}/Auxiliary/FltkImageViewer
${GradientRecursiveGaussianFilter_SOURCE_DIR}
${GradientRecursiveGaussianFilter_BINARY_DIR}
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
${GLU_INCLUDE_PATH}
)

ADD_EXECUTABLE(GradientRecursiveGaussianFilter
  GradientRecursiveGaussianFilterBase.cxx
  GradientRecursiveGaussianFilter.cxx
  main.cxx
)

FLTK_WRAP_UI(GradientRecursiveGaussianFilter 
  GradientRecursiveGaussianFilterGUI.fl
)

TARGET_LINK_LIBRARIES(GradientRecursiveGaussianFilter
                      ITKBasicFilters ITKIO ITKFltkImageViewer
                      ${FLTK_LIBRARY} ${OPENGL_LIBRARY} ${GLU_LIBRARY})


