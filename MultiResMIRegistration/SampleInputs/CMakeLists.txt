CONFIGURE_FILE(
${ITK_SOURCE_DIR}/Examples/MultiResMIRegistration/SampleInputs/VVBatch.txt
${ITK_BINARY_DIR}/Examples/MultiResMIRegistration/SampleInputs/VVBatch.txt
COPYONLY )

CONFIGURE_FILE(
${ITK_SOURCE_DIR}/Examples/MultiResMIRegistration/SampleInputs/VVCTToMR.txt
${ITK_BINARY_DIR}/Examples/MultiResMIRegistration/SampleInputs/VVCTToMR.txt
COPYONLY )

CONFIGURE_FILE(
${ITK_SOURCE_DIR}/Examples/MultiResMIRegistration/SampleInputs/VVMRToPET.txt
${ITK_BINARY_DIR}/Examples/MultiResMIRegistration/SampleInputs/VVMRToPET.txt
COPYONLY )
