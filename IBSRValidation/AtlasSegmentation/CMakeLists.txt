PROJECT( AtlasSegmentation )

INCLUDE_DIRECTORIES(
${AtlasSegmentation_SOURCE_DIR}/../Common
${AtlasSegmentation_SOURCE_DIR}
)

ADD_EXECUTABLE(BrainStripValidationApp Code/BrainStripValidationApp.cxx)
TARGET_LINK_LIBRARIES(BrainStripValidationApp ITKBasicFilters ITKIO)



