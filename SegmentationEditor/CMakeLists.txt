INCLUDE_REGULAR_EXPRESSION("^.*$")

INCLUDE (${ITK_SOURCE_DIR}/Examples/SegmentationEditor/WST.cmake)

# Add the common directory
SUBDIRS ( Common )
SUBDIRS ( Wrapping )
