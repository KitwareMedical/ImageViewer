INCLUDE_REGULAR_EXPRESSION(".*")

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Examples/MultiResMIRegistration/Common
${ITK_SOURCE_DIR}/Examples/IBSRValidation/Common
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/IO
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Code/Numerics
)


LINK_LIBRARIES (
VXLNumerics
ITKNumerics
ITKCommon
ITKIO
ITKBasicFilters 
)

ADD_EXECUTABLE(MultiResMIRegistration Common/MultiResMIRegistration )

