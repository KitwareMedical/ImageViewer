PROJECT( FuzzyConnectedness )
INCLUDE_DIRECTORIES(
${FuzzyConnectedness_SOURCE_DIR}
${FuzzyConnectedness_SOURCE_DIR}/../IBSRValidation/Common
)

SET( FuzzyConnect_SRCS
FuzzyConnectApp.cxx
FuzzyConnectedness.cxx
)


ADD_EXECUTABLE(FuzzyConnectedness ${FuzzyConnect_SRCS} )
TARGET_LINK_LIBRARIES(FuzzyConnectedness VXLNumerics
ITKCommon
ITKIO
)

