INCLUDE_DIRECTORIES(
  ${InsightApplications_SOURCE_DIR}/MultichannelTissueClassificationValidation/Common
  ${InsightApplications_SOURCE_DIR}/MultichannelTissueClassificationValidation/Code
)

ADD_EXECUTABLE(GaussianMSClassificationApp
               Code/GaussianMSClassifierValidationApp.cxx)
ADD_EXECUTABLE(KmeansMSClassificationApp 
               Code/KmeansMSClassifierValidationApp.cxx)
#ADD_EXECUTABLE(MRFGaussianIBSRClassificationApp 
# Code/MRFGaussianClassifierValidationApp.cxx)
#ADD_EXECUTABLE(MRFKmeansIBSRClassificationApp 
# Code/MRFKmeansClassifierValidationApp.cxx)

TARGET_LINK_LIBRARIES(GaussianMSClassificationApp ITKBasicFilters ITKIO)
TARGET_LINK_LIBRARIES(KmeansMSClassificationApp ITKBasicFilters ITKIO)
