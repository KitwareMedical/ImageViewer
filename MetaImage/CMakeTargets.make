INCLUDE_FLAGS = -I/e/src/Insight -I/e/src/Insight/Code/Insight3DParty/vxl -I/e/src/Insight/Code/Common -I/e/src/Insight/Code/Insight3DParty/vxl  ${LOCAL_INCLUDE_FLAGS .} 
LIBRARY = \
MetaImageIO

BUILD_LIB_FILE = lib${LIBRARY}${CMAKE_LIB_EXT}

SRC_OBJ = \
MetaFileLib.o \
MetaImageLib.o \
itkWriteMetaImage.o \
MetaUtils.o 




