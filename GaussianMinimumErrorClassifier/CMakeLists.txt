INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Examples/MRIBiasCorrection
${ITK_SOURCE_DIR}/Examples/GaussianMinimumErrorClassifier
)

LINK_LIBRARIES (
ITKBasicFilters
ITKIO
)

SET (GaussianMinimumErrorClassifier_SRCS
${ITK_SOURCE_DIR}/Examples/MRIBiasCorrection/OptionList.cxx
MaximumLikelihoodRatioDecisionRule.cxx
GaussianMinimumErrorClassifier.cxx
)

ADD_EXECUTABLE(GaussianMinimumErrorClassifier ${GaussianMinimumErrorClassifier_SRCS})
