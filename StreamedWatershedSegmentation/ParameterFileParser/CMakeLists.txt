# Sources of non-templated classes.
SOURCE_FILES(param_SRCS
lex.yy.cxx
y.tab.cxx
param.cxx
)

ADD_LIBRARY(param param_SRCS)
