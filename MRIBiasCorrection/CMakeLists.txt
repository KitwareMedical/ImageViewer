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
OptionList.cxx
ImageCorrector.cxx
)
 

SOURCE_FILES(BiasImageGenerator_SRCS
OptionList.cxx
BiasImageGenerator.cxx
)
 
SOURCE_FILES(BiasFieldEstimator_SRCS
OptionList.cxx
BiasFieldEstimator.cxx
)

SOURCE_FILES(InterSliceIntensityCorrector_SRCS
OptionList.cxx
InterSliceIntensityCorrector.cxx
)

SOURCE_FILES(IntensityHistogram3D_SRCS
OptionList.cxx
IntensityHistogram3D.cxx
)

SOURCE_FILES(SlabIdentifier_SRCS
OptionList.cxx
SlabIdentifier.cxx
)

SOURCE_FILES(EnergyTablePlotter_SRCS
OptionList.cxx
EnergyTablePlotter.cxx
)


SOURCE_FILES(BiasCorrector_SRCS
OptionList.cxx
BiasCorrector.cxx
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




