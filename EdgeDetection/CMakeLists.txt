INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Examples/EdgeDetection
)

LINK_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Code/IO
)

LINK_LIBRARIES (
ITKBasicFilters
ITKCommon
VXLNumerics
ITKIO
)

#ADD_EXECUTABLE(itk2DEdgeDetectionSample itk2DEdgeDetectionSample.cxx)
ADD_EXECUTABLE(itk3DEdgeDetectionSample itk3DEdgeDetectionSample.cxx)
