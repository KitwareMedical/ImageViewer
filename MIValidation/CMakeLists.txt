INCLUDE_REGULAR_EXPRESSION(".*")

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Examples/MultiResMIRegistration/Common
${ITK_SOURCE_DIR}/Examples/IBSRValidation/Common
)

SET( MIValidation_SRCS
Code/MIValidationApp.cxx
Code/VanderbiltModality.cxx )

ADD_EXECUTABLE(MIValidationApp   ${MIValidation_SRCS} )
TARGET_LINK_LIBRARIES(MIValidationApp ITKNumerics ITKIO ITKBasicFilters)

