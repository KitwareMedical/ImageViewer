INCLUDE_REGULAR_EXPRESSION(".*")

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Examples/MultiResMIRegistration/Common
${ITK_SOURCE_DIR}/Examples/IBSRValidation/Common
)

ADD_EXECUTABLE(MultiResMIRegistration Common/MultiResMIRegistration.cxx )
TARGET_LINK_LIBRARIES(MultiResMIRegistration ITKNumerics ITKIO ITKBasicFilters)
