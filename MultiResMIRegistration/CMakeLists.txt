PROJECT( MultiResMIRegistration )

INCLUDE_REGULAR_EXPRESSION(".*")

INCLUDE_DIRECTORIES(
${MultiResMIRegistration_SOURCE_DIR}/Common
${MultiResMIRegistration_SOURCE_DIR}/../IBSRValidation/Common
)

ADD_GUI_EXECUTABLE(MultiResMIRegistration Common/MultiResMIRegistration.cxx )
TARGET_LINK_LIBRARIES(MultiResMIRegistration ITKNumerics ITKIO ITKBasicFilters)
