
# Build application for altas-based segmentation of brain volume
SUBDIRS( AtlasSegmentation )

# Build FEM application for altas-based segmentation of brain volume
# Disabled because example is out of date while releasing ITK 1.0.
#SUBDIRS( FEMAtlasSegmentation )

# Build application for gray, white matter classification of brain volume
SUBDIRS( IBSRClassification )
