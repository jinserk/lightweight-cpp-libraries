.SUFFIXES: .cpp .cc .c .hpp .hh .h .o

CC = gcc
CXX = g++
RM = rm
AR = ar

SRCS_DIR = ../src

# .c files
CFILES = \
#main.c \

CSRCS = $(CFILES:%=$(SRCS_DIR)/%)
SRCS = $(CSRCS)
OBJS = $(CSRCS:.c=.o)

# .cpp files
CCFILES = \
#main.cc \

CCSRCS = $(CCFILES:%=$(SRCS_DIR)/%)
SRCS += $(CCSRCS)
OBJS += $(CCSRCS:.cc=.o)

# .cpp files
CPPFILES = \
framework.cpp \

CPPSRCS += $(CPPFILES:%=$(SRCS_DIR)/%)
SRCS += $(CPPSRCS)
OBJS += $(CPPSRCS:.cpp=.o)

# flags
INCS = -I.
INCS += -I../include

LIBS = -L.
LIBS += -lm -lstdc++

CFLAGS = -std=c99 -O3 -Wall -Wno-deprecated -g
CXXFLAGS = -std=c++98 -O3 -Wall -Wno-deprecated -g
LDFLAGS = #-static #-dynamiclib

GENDEPFLAGS = -MM

TARGET_DIR = ../lib
TARGET = libframework.a

all : $(TARGET)

$(TARGET): $(OBJS)
	$(AR) -r $(TARGET_DIR)/$@ $(OBJS)

.c.o:
	$(CC) $(INCS) $(CFLAGS) -c $< -o $@

.cc.o:
	$(CXX) $(INCS) $(CXXFLAGS) -c $< -o $@

.cpp.o:
	$(CXX) $(INCS) $(CXXFLAGS) -c $< -o $@

clean :
	$(RM) $(OBJS)
	$(RM) $(TARGET_DIR)/$(TARGET)

new :
	$(MAKE) clean
	$(MAKE)

.PHONY : all clean
