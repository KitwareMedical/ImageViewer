PROJECT (IMAGETCL)

INCLUDE (${CMAKE_ROOT}/Modules/FindTCL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGCCXML.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindCABLE.cmake)

INCLUDE_DIRECTORIES (
  ${TCL_INCLUDE_PATH}
  ${CABLE_INCLUDE_DIR}
  ${ITK_SOURCE_DIR}/Code/Common
  ${ITK_SOURCE_DIR}/Code/BasicFilters
  ${ITK_SOURCE_DIR}/Code/Algorithms
  ${ITK_SOURCE_DIR}/Code/IO
  ${ITK_SOURCE_DIR}/Examples/TclWrappedImage
  )

LINK_LIBRARIES (
  ${TCL_LIBRARY}
  ${CABLE_TCL_LIBRARY}
  ${ITK_BINARY_DIR}/Code/Common
  ${ITK_BINARY_DIR}/Code/BasicFilters
  ${ITK_BINARY_DIR}/Code/Algorithms
  ${ITK_BINARY_DIR}/Code/IO
  ) 

ADD_LIBRARY (ImageTcl SHARED)

# Each class follows the format:
# [tag:]class_name[;source1;source2;...]
#  
CABLE_WRAP_TCL ( ImageTcl 
    "itkVector3D:itk::Vector<double,3>;itkVector.h"
    "itkLightObject:itkLightObject;itkLightObject.h"
    "itkObject:itk::Object;itkObject.h"
    "itkDataObject:itkDataObject;itkDataObject.h"
    "itkSize3D:itk::Size<3>:itkSize.h"
    "itkIndex3D:itk::Index<3>:itkIndex.h"
    "itkArray3DULong:itk::Array<unsigned long,3>;itkArray.h"
    "itkArray3DDouble:itk::Array<double,3>;itkArray.h"
    "itkArray3DInt:itk::Array<int,3>;itkArray.h"
    "itkImage3DBase:itk::ImageBase<3>;itkImageBase.h"
    "itkImage3DChar:itk::Image<char,3>;itkImage.h"
    "itkImage3DRegion:itk::ImageRegion<3>;itkImageRegion.h" 
    "itkImage3DCharPointer:itk::Image<char,3>::Pointer;itkImage.h"
    )

# Configure files
CONFIGURE_FILE(
${ITK_SOURCE_DIR}/Examples/TclWrappedImage/itkImage.tcl.in 
${ITK_BINARY_DIR}/Examples/TclWrappedImage/itkImage.tcl 
)



