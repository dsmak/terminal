EXE=server

SOURCE_DIRS := . ../src 
SEARCH_WILDCARDS := $(addsuffix /*.c,$(SOURCE_DIRS)) 

OBJS := $(notdir $(patsubst %.c,%.o,$(wildcard $(SEARCH_WILDCARDS))))

##########################################
# Compilers
##########################################
CC=gcc

##########################################
# json-c specific variables
##########################################
JSINCLUDE=-I../libs/json-c

##########################################
# INCLUDE
##########################################
INCLUDE_C=-I. -I/usr/include 

##########################################
# C/C++ FLAGS
##########################################
CFLAGS=-lmicrohttpd -ljson-c -std=c99

build:	$(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(CFLAGS)

.PHONY:clean
clean:
	-rm $(OBJS)
	-rm $(EXE)
	-rm *.d

VPATH := $(SOURCE_DIRS)
#
#
.SUFFIXES: .c .o .cpp 

.c.o:
	$(CC) $(CFLAGS)  -c -MD -w $(INCLUDE_C) $(JSINCLUDE) $(addprefix -I, $(SOURCE_DIRS)) $<

include $(wildcard *.d)

