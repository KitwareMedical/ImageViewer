INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Applications/MRIBiasCorrection
)

SET(ImageCorrector_SRCS
OptionList.cxx
ImageCorrector.cxx
)
 

SET(BiasImageGenerator_SRCS
OptionList.cxx
BiasImageGenerator.cxx
)
 
SET(BiasFieldEstimator_SRCS
OptionList.cxx
BiasFieldEstimator.cxx
)

SET(InterSliceIntensityCorrector_SRCS
OptionList.cxx
InterSliceIntensityCorrector.cxx
)

SET(IntensityHistogram3D_SRCS
OptionList.cxx
IntensityHistogram3D.cxx
)

SET(SlabIdentifier_SRCS
OptionList.cxx
SlabIdentifier.cxx
)

SET(EnergyTablePlotter_SRCS
OptionList.cxx
EnergyTablePlotter.cxx
)


SET(BiasCorrector_SRCS
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
ITKIO
ITKBasicFilters
ITKStatistics
ITKNumerics
)


ADD_EXECUTABLE(BiasFieldEstimator ${BiasFieldEstimator_SRCS})
ADD_EXECUTABLE(BiasImageGenerator ${BiasImageGenerator_SRCS})
ADD_EXECUTABLE(IntensityHistogram3D ${IntensityHistogram3D_SRCS})
ADD_EXECUTABLE(InterSliceIntensityCorrector ${InterSliceIntensityCorrector_SRCS})
ADD_EXECUTABLE(SlabIdentifier ${SlabIdentifier_SRCS})
ADD_EXECUTABLE(EnergyTablePlotter ${EnergyTablePlotter_SRCS})
ADD_EXECUTABLE(ImageCorrector ${ImageCorrector_SRCS})
ADD_EXECUTABLE(BiasCorrector ${BiasCorrector_SRCS})




