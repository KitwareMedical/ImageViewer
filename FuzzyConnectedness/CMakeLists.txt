
INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Examples/MultiResMIRegistration
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
)


LINK_LIBRARIES (
VXLNumerics
ITKCommon
)

SOURCE_FILES( FuzzyConnect_SRCS
FuzzyConnectApp
FuzzyConnectedness )


ADD_EXECUTABLE(FuzzyConnectedness FuzzyConnect_SRCS )

