PROJECT( IBSRClassification )

INCLUDE_DIRECTORIES(
${IBSRClassification_SOURCE_DIR}/../Common
${IBSRClassification_SOURCE_DIR}
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

