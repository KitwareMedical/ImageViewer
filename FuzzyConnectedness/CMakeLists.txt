INCLUDE_DIRECTORIES(${InsightApplications_SOURCE_DIR}/FuzzyConnectedness
                    ${InsightApplications_SOURCE_DIR}/IBSRValidation/Common)

ADD_EXECUTABLE(FuzzyConnectedness FuzzyConnectApp.cxx FuzzyConnectedness.cxx)

TARGET_LINK_LIBRARIES(FuzzyConnectedness ITKIO)
