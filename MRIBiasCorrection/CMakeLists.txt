INCLUDE_DIRECTORIES(${InsightApplications_SOURCE_DIR}/MRIBiasCorrection)

FOREACH(exe BiasFieldEstimator BiasImageGenerator IntensityHistogram3D
            InterSliceIntensityCorrector SlabIdentifier EnergyTablePlotter
            ImageCorrector BiasCorrector)
  ADD_EXECUTABLE(${exe} ${exe}.cxx OptionList.cxx)
  TARGET_LINK_LIBRARIES(${exe} ITKIO ITKBasicFilters ITKStatistics ITKNumerics)
ENDFOREACH(exe)
