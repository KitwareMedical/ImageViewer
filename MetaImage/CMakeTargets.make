INCLUDE_FLAGS = -I../../.. -I../../../Code/Insight3DParty/vxl -I../../../Code/Common ${LOCAL_INCLUDE_FLAGS .} 
LIBRARY = \
MetaImageIO

BUILD_LIB_FILE = lib${LIBRARY}${CMAKE_LIB_EXT}

SRC_OBJ = \
MetaFileLib.o \
MetaImageLib.o \
itkWriteMetaImage.o \
MetaUtils.o 




