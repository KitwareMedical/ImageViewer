INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
)

LINK_LIBRARIES (
VXLNumerics
ITKCommon
ITKBasicFilters 
)

UNIX_LIBRARIES (
-lm
)

EXECUTABLES(itkPrincipalAxesResampler)

