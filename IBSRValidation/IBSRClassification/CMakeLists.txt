
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
ADD_EXECUTABLE(MRFGaussianIBSRClassificationApp 
 Code/MRFGaussianClassifierValidationApp.cxx)
ADD_EXECUTABLE(MRFKmeansIBSRClassificationApp 
 Code/MRFKmeansClassifierValidationApp.cxx)

