PROJECT (STRINGTCL)

INCLUDE (${CMAKE_ROOT}/Modules/FindTCL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGCCXML.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindCABLE.cmake)

INCLUDE_DIRECTORIES (
      ${TCL_INCLUDE_PATH}
      ${CABLE_INCLUDE_DIR}
  )

LINK_LIBRARIES (${TCL_LIBRARY}
${CABLE_TCL_LIBRARY})

ADD_LIBRARY (StringTcl SHARED)
CABLE_WRAP_TCL (StringTcl std::string;string)

# Configure files
CONFIGURE_FILE(
${ITK_SOURCE_DIR}/Examples/TclWrappedSTLString/stdString.tcl.in 
${ITK_BINARY_DIR}/Examples/TclWrappedSTLString/stdString.tcl 
)



