INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
)

SUBDIRS(MultiResMIRegistration)


LINK_LIBRARIES (
VXLNumerics
ITKCommon
ITKBasicFilters 
)

IF(UNIX)
  LINK_LIBRARIES (
  -lm
  )
ENDIF(UNIX)


ADD_EXECUTABLE(itkPrincipalAxesResampler itkPrincipalAxesResampler)

