
INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Examples/IBSRValidation/Common
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

