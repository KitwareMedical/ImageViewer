INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/Numerics
${ITK_SOURCE_DIR}/Code/Numerics/Statistics
${ITK_SOURCE_DIR}/Auxiliary/MetaImage
${ITK_SOURCE_DIR}/Examples/MRIBiasCorrection
)

SOURCE_FILES(ImageCorrector_SRCS
OptionList
${ITK_SOURCE_DIR}/Code/Numerics/itkMultivariateLegendrePolynomial
ImageCorrector
)
 

SOURCE_FILES(BiasImageGenerator_SRCS
OptionList
${ITK_SOURCE_DIR}/Code/Numerics/itkMultivariateLegendrePolynomial
BiasImageGenerator
)
 
SOURCE_FILES(BiasFieldEstimator_SRCS
OptionList
${ITK_SOURCE_DIR}/Code/Numerics/itkCacheableScalarFunction
${ITK_SOURCE_DIR}/Code/Numerics/itkCompositeValleyFunction
${ITK_SOURCE_DIR}/Code/Numerics/itkMultivariateLegendrePolynomial
BiasFieldEstimator
)

SOURCE_FILES(InterSliceIntensityCorrector_SRCS
OptionList
${ITK_SOURCE_DIR}/Code/Numerics/itkCacheableScalarFunction
${ITK_SOURCE_DIR}/Code/Numerics/itkCompositeValleyFunction
${ITK_SOURCE_DIR}/Code/Numerics/itkMultivariateLegendrePolynomial
InterSliceIntensityCorrector
)

SOURCE_FILES(IntensityHistogram3D_SRCS
OptionList
IntensityHistogram3D
)

SOURCE_FILES(SlabIdentifier_SRCS
OptionList
SlabIdentifier
)

SOURCE_FILES(EnergyTablePlotter_SRCS
OptionList
${ITK_SOURCE_DIR}/Code/Numerics/itkCacheableScalarFunction
${ITK_SOURCE_DIR}/Code/Numerics/itkCompositeValleyFunction
EnergyTablePlotter
)


SOURCE_FILES(BiasCorrector_SRCS
OptionList
${ITK_SOURCE_DIR}/Code/Numerics/itkCacheableScalarFunction
${ITK_SOURCE_DIR}/Code/Numerics/itkCompositeValleyFunction
${ITK_SOURCE_DIR}/Code/Numerics/itkMultivariateLegendrePolynomial
BiasCorrector
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Numerics/Statistics
${ITK_BINARY_DIR}/Auxiliary/MetaImage
)


LINK_LIBRARIES (
ITKCommon
ITKIO
ITKBasicFilters
ITKStatistics
MetaImageIO
VXLNumerics
)


ADD_EXECUTABLE(BiasFieldEstimator BiasFieldEstimator_SRCS)
ADD_EXECUTABLE(BiasImageGenerator BiasImageGenerator_SRCS)
ADD_EXECUTABLE(IntensityHistogram3D IntensityHistogram3D_SRCS)
ADD_EXECUTABLE(InterSliceIntensityCorrector InterSliceIntensityCorrector_SRCS)
ADD_EXECUTABLE(SlabIdentifier SlabIdentifier_SRCS)
ADD_EXECUTABLE(EnergyTablePlotter EnergyTablePlotter_SRCS)
ADD_EXECUTABLE(ImageCorrector ImageCorrector_SRCS)
ADD_EXECUTABLE(BiasCorrector BiasCorrector_SRCS)




