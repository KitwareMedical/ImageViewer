
INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Examples/MultiResMIRegistration
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/IO
${ITK_BINARY_DIR}/Code/Common
)


LINK_LIBRARIES (
VXLNumerics
ITKIO
ITKCommon
)

SOURCE_FILES( MIRegistration_SRCS
MIRegistrationApp
MultiResMIRegistration )


ADD_EXECUTABLE(MultiResMIRegistration MIRegistration_SRCS )

