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


#
#  Definitions equivalent to C-typedefs
#
CABLE_CLASS_SET( PixelType                "unsigned char"                          )
CABLE_CLASS_SET( Dimension                  3                                      )
CABLE_CLASS_SET( itkImage3DBase           "itk::ImageBase<$Dimension>"             )
CABLE_CLASS_SET( itkImage3DUchar          "itk::Image<$PixelType,$Dimension>"      )
CABLE_CLASS_SET( itkImage3DUcharPointer   "itk::SmartPointer< $itkImage3DUchar >"  )
CABLE_CLASS_SET( itkVector3Ddouble        "itk::Vector<double, $Dimension>"        )
CABLE_CLASS_SET( itkImage3DRegion         "itk::ImageRegion<$Dimension>"           )
CABLE_CLASS_SET( itkSize3DType            "itk::Size<$Dimension>"                  )
CABLE_CLASS_SET( itkIndex3DType           "itk::Index<$Dimension>"                 )


#
# Each class follows the format:
# [tag:]class_name[;source1;source2;...]
#  
CABLE_WRAP_TCL ( ImageTcl 
    "$itkImage3DBase;itkImageBase.h"
    "$itkImage3DUchar;itkImage.h"
    "$itkImage3DUcharPointer;itkImage.h"
    "$itkVector3Ddouble;itkVector.h"
    "$itkImage3DRegion;itkImageRegion.h"
    "$itkSize3DType;itkSize.h"
    "$itkIndex3DType;itkIndex.h"
    )

# Configure files
CONFIGURE_FILE(
${ITK_SOURCE_DIR}/Examples/TclWrappedImage/itkImage.tcl.in 
${ITK_BINARY_DIR}/Examples/TclWrappedImage/itkImage.tcl 
)



