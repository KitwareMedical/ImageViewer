INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/Code/Numerics/vxl
${CMAKE_SOURCE_DIR}/Code/Numerics/vxl/vcl
${CMAKE_SOURCE_DIR}/Code/Common
${CMAKE_SOURCE_DIR}/Code/BasicFilters
${CMAKE_SOURCE_DIR}/Code/Algorithms
)

LINK_DIRECTORIES(
${CMAKE_BINARY_DIR}/Code/Numerics/vxl
${CMAKE_BINARY_DIR}/Code/Common
${CMAKE_BINARY_DIR}/Code/BasicFilters
${CMAKE_BINARY_DIR}/Code/Algorithms
)

LINK_LIBRARIES (
ITKBasicFilters 
ITKCommon
VXLNumerics
)

EXECUTABLES(itkPrincipalAxesResampler)
