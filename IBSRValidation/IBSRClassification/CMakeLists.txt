
INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Examples/IBSRValidation/Common
${ITK_SOURCE_DIR}/Examples/IBSRValidation/IBSRClassification
)

LINK_LIBRARIES (
ITKBasicFilters
ITKIO
)

ADD_EXECUTABLE(GaussianIBSRClassificationApp 
 Code/GaussianClassifierValidationApp.cxx)
ADD_EXECUTABLE(KmeansIBSRClassificationApp 
 Code/KmeansClassifierValidationApp.cxx)


