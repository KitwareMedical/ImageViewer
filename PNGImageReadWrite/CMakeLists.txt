SET(PNGImageReadWrite_SRCS
itkPNGImageReadWrite.cxx
)



ADD_EXECUTABLE(PNGImageReadWrite      ${PNGImageReadWrite_SRCS})
TARGET_LINK_LIBRARIES (PNGImageReadWrite ITKIO)


