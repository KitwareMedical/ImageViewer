INCLUDE_REGULAR_EXPRESSION(".*")

SUBDIRS( SampleInputs )

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Examples/MultiResMIRegistration/Common
${ITK_SOURCE_DIR}/Examples/MultiResMIRegistration/VanderbiltValdiation
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

SOURCE_FILES( VanderbiltValidation_SRCS
VanderbiltValidation/VanderbiltValidation
VanderbiltValidation/VanderbiltModality )


ADD_EXECUTABLE(MultiResMIRegistration Common/MultiResMIRegistration )
IF(UNIX)
ADD_EXECUTABLE(VanderbiltValidation/VanderbiltValidation   VanderbiltValidation_SRCS )
ELSE(UNIX)
ADD_EXECUTABLE(VanderbiltValidation   VanderbiltValidation_SRCS )
ENDIF(UNIX)
