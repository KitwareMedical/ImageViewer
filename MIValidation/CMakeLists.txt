PROJECT( MIValidation )

INCLUDE_REGULAR_EXPRESSION(".*")

INCLUDE_DIRECTORIES(
${MIValidation_SOURCE_DIR}/../MultiResMIRegistration/Common
${MIValidation_SOURCE_DIR}/../IBSRValidation/Common
)

SET( MIValidation_SRCS
Code/MIValidationApp.cxx
Code/VanderbiltModality.cxx )

ADD_EXECUTABLE(MIValidationApp   ${MIValidation_SRCS} )
TARGET_LINK_LIBRARIES(MIValidationApp ITKNumerics ITKIO ITKBasicFilters)

