PROJECT(AntiAliasBinaryImageFilter)

INCLUDE_DIRECTORIES(
${AntiAliasBinaryImageFilter_SOURCE_DIR}
)

LINK_LIBRARIES (
param
ITKBasicFilters
ITKCommon
VXLNumerics
ITKIO
)

ADD_EXECUTABLE(itkAntiAliasBinaryImageFilterExample itkAntiAliasBinaryImageFilterExample.cxx)
ADD_EXECUTABLE(itkSampleDataGenerator itkSampleDataGenerator.cxx)
