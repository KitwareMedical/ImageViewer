# Sources of non-templated classes.
SOURCE_FILES(chunk_SRCS
chunks.cxx
)

ADD_LIBRARY(chunks chunk_SRCS)

INCLUDE_DIRECTORIES(
${StreamedWatershedSegmentation_SOURCE_DIR}/ParameterFileParser
${StreamedWatershedSegmentation_SOURCE_DIR}/VolumeChunker
)

LINK_DIRECTORIES(
${StreamedWatershedSegmentation_SOURCE_DIR}/ParameterFileParser
${StreamedWatershedSegmentation_SOURCE_DIR}/VolumeChunker
)

LINK_LIBRARIES (
chunks
param
)

ADD_EXECUTABLE(catchunkfile catchunkfile)
ADD_EXECUTABLE(chunker chunker)
ADD_EXECUTABLE(vhchunker vhchunker)
ADD_EXECUTABLE(vhunchunker vhunchunker)
