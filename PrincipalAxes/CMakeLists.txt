
INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/IO
)


LINK_LIBRARIES (
VXLNumerics
ITKCommon
ITKIO

)


ADD_EXECUTABLE(itkPrincipalAxesResampler itkPrincipalAxesResampler.cxx )

