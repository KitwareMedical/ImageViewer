
# Build application for altas-based segmentation of brain volume
SUBDIRS( AtlasSegmentation )

SET(HAS_FEM 0 CACHE INTERNAL "Support for FEM")
OPTION(BUILD_FEM_CODE "Build the FEM Code" ON)
IF(BUILD_FEM_CODE)
# Build FEM application for altas-based segmentation of brain volume
  SUBDIRS( FEMAtlasSegmentation )
  SET(HAS_FEM 1 CACHE INTERNAL "Support for FEM")
ENDIF(BUILD_FEM_CODE)

# Build application for gray, white matter classification of brain volume
SUBDIRS( IBSRClassification )