INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Numerics
${ITK_SOURCE_DIR}/Code/Numerics/Statistics
${ITK_SOURCE_DIR}/Unsupported/MetaImage
${ITK_SOURCE_DIR}/Examples/MRIBiasCorrection
)

SOURCE_FILES(BiasCorrector_SRCS
OptionList
BiasField
BiasCorrector
)

SOURCE_FILES(BiasImageGenerator_SRCS
OptionList
BiasField
BiasImageGenerator
)

SOURCE_FILES(BiasFieldEstimator_SRCS
OptionList
BiasField
EnergyTable
BiasFieldEstimator
)

SOURCE_FILES(InterSliceIntensityCorrector_SRCS
OptionList
BiasField
EnergyTable
InterSliceIntensityCorrector
)


SOURCE_FILES(IntensityHistogram3D_SRCS
OptionList
IntensityHistogram3D
)


SOURCE_FILES(EnergyTablePlotter_SRCS
OptionList
EnergyTable
EnergyTablePlotter
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Numerics/Statistics
${ITK_BINARY_DIR}/Unsupported/MetaImage
)


LINK_LIBRARIES (
ITKCommon
ITKBasicFilters
ITKStatistics
MetaImageIO
)


ADD_EXECUTABLE(BiasFieldEstimator BiasFieldEstimator_SRCS)
ADD_EXECUTABLE(BiasCorrector BiasCorrector_SRCS)
ADD_EXECUTABLE(BiasImageGenerator BiasImageGenerator_SRCS)
ADD_EXECUTABLE(InterSliceIntensityCorrector InterSliceIntensityCorrector_SRCS)
ADD_EXECUTABLE(IntensityHistogram3D IntensityHistogram3D_SRCS)
ADD_EXECUTABLE(EnergyTablePlotter EnergyTablePlotter_SRCS)



