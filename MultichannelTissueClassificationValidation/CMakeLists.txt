INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Examples/MultichannelTissueClassificationValidation/Common
${ITK_SOURCE_DIR}/Examples/MultichannelTissueClassificationValidation/Code
)

LINK_LIBRARIES (
ITKBasicFilters
ITKIO
)

ADD_EXECUTABLE(GaussianMSClassificationApp 
 Code/GaussianMSClassifierValidationApp.cxx)
ADD_EXECUTABLE(KmeansMSClassificationApp 
 Code/KmeansMSClassifierValidationApp.cxx)
#ADD_EXECUTABLE(MRFGaussianIBSRClassificationApp 
# Code/MRFGaussianClassifierValidationApp.cxx)
#ADD_EXECUTABLE(MRFKmeansIBSRClassificationApp 
# Code/MRFKmeansClassifierValidationApp.cxx)



