PROJECT( MultichannelTissueClassificationValidation )

INCLUDE_DIRECTORIES(
${MultichannelTissueClassificationValidation_SOURCE_DIR}/Common
${MultichannelTissueClassificationValidation_SOURCE_DIR}/Code
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



