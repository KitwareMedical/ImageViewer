##########################################
#
#     MetaImageLib
#
#     University of North Carolina
#     Chapel Hill, NC 27514
#
########################################


RESULT = $(HOME_LIB)/libMetaImage.a



#################################################
#
#	Suffixes used 
#
#################################################

.SUFFIXES: .o .cpp .cxx .h


#################################################
#
#	Include Directories
#
#################################################

INCLUDES =      -I.                         \
                $(METAIMAGELIB_INCLUDE)     \
                $(INSIGHT_INCLUDE)          \
                $(VNL_INCLUDE)


#################################################
#
#	Compiler Options
#
#################################################

COPTIONS =      -Wall   -O3

COMPOPTIONS =   $(COPTIONS)   



################################################
#
#	Implicit Rules
#
################################################

.cpp.o:
	$(CXX) -c  $<   $(INCLUDES) $(COMPOPTIONS)


.cxx.o:
	$(CXX) -c  $<   $(INCLUDES) $(COMPOPTIONS)




################################################
#
#	Object Files
#
################################################

OBJS =	MetaFileLib.o           \
        MetaImageLib.o          \
        MetaUtils.o             \
        itkWriteMetaImage.o




#######################################################
#
#	Main target : create the library
#
#######################################################

all:	$(RESULT)



#######################################################
#
#	Remove all targets
#
#######################################################

clean: 
	rm *.o
	rm $(RESULT)




########################################################
#
#	Librarian commands
#
########################################################

$(RESULT):	$(OBJS)
	ar -uvr $(RESULT)  $(OBJS) 



