INCLUDE_REGULAR_EXPRESSION("^.*$")

INCLUDE (${ITK_SOURCE_DIR}/Examples/WST/WST.cmake)

# Add the common directory
SUBDIRS ( Common )
SUBDIRS ( Wrapping )
