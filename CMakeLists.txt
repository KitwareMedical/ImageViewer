INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}/Code/BasicFilters
${CMAKE_SOURCE_DIR}/Code/Algorithms
)

LINK_DIRECTORIES(
${CMAKE_BINARY_DIR}/Code/BasicFilters
${CMAKE_BINARY_DIR}/Code/Algorithms
)

LINK_LIBRARIES (
VXLNumerics
ITKCommon
ITKBasicFilters 
)

EXECUTABLES(itkPrincipalAxesResampler)

