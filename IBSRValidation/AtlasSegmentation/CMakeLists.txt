
INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Examples/IBSRValidation/Common
${ITK_SOURCE_DIR}/Examples/IBSRValidation/AtlasSegmentation
)

ADD_EXECUTABLE(BrainStripValidationApp Code/BrainStripValidationApp.cxx)
TARGET_LINK_LIBRARIES(BrainStripValidationApp ITKBasicFilters ITKIO)



