
PROJECT(BasicIOAndFilter)

#  
# Find ITK
#
# Uncomment the following lines if you want to
# build this code outside the ITK binary tree  
# INCLUDE (${CMAKE_ROOT}/Modules/FindITK.cmake)
# IF (USE_ITK_FILE)
#   INCLUDE (${USE_ITK_FILE})
# ENDIF (USE_ITK_FILE)


#specify executables
ADD_EXECUTABLE( BasicIOAndFilter   BasicIOAndFilter.cxx )
TARGET_LINK_LIBRARIES( BasicIOAndFilter ITKIO )
