INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/Numerics
${ITK_SOURCE_DIR}/Code/Numerics/Statistics
${ITK_SOURCE_DIR}/Examples/MRIBiasCorrection
)

SOURCE_FILES(ImageCorrector_SRCS
OptionList
ImageCorrector
)
 

SOURCE_FILES(BiasImageGenerator_SRCS
OptionList
BiasImageGenerator
)
 
SOURCE_FILES(BiasFieldEstimator_SRCS
OptionList
BiasFieldEstimator
)

SOURCE_FILES(InterSliceIntensityCorrector_SRCS
OptionList
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
EnergyTablePlotter
)


SOURCE_FILES(BiasCorrector_SRCS
OptionList
BiasCorrector
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Numerics
${ITK_BINARY_DIR}/Code/Numerics/Statistics
${ITK_BINARY_DIR}/Code/IO
)


LINK_LIBRARIES (
ITKCommon
ITKIO
ITKBasicFilters
ITKStatistics
ITKNumerics
MetaIO
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




