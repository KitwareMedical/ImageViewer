INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Numerics
${ITK_SOURCE_DIR}/Code/Numerics/Statistics
${ITK_SOURCE_DIR}/Auxiliary/MetaImage
${ITK_SOURCE_DIR}/Examples/MRIBiasCorrection
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Numerics/vxl
${ITK_BINARY_DIR}/Auxilary/MetaImage
)


LINK_LIBRARIES (
VXLNumerics
ITKCommon
ITKBasicFilters
ITKIO
MetaImageIO
)

SOURCE_FILES (GaussianMinimumErrorClassifier_SRCS
${ITK_SOURCE_DIR}/Examples/MRIBiasCorrection/OptionList
${ITK_SOURCE_DIR}/Code/Numerics/Statistics/itkMaxDecisionRule
GaussianMinimumErrorClassifier
)

ADD_EXECUTABLE(GaussianMinimumErrorClassifier GaussianMinimumErrorClassifier_SRCS)
