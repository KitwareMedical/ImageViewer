# Sources of non-templated classes.
SOURCE_FILES(param_SRCS
lex.yy.cxx
y.tab.cxx
param.cxx
)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
)

LINK_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
)

LINK_LIBRARIES (
ITKCommon
)

ADD_LIBRARY(param param_SRCS)
