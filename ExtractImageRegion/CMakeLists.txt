LINK_LIBRARIES (
  ITKBasicFilters
  ITKIO
    )

IF(WIN32)
  LINK_LIBRARIES(
    wsock32
  )
ENDIF(WIN32)

ADD_EXECUTABLE( ExtractImageRegion ExtractImageRegion.cxx )
