.SUFFIXES: .cpp .cc .c .hpp .hh .h .o

CC = gcc
CXX = g++
RM = rm

SRCS_DIR = .

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
main.cpp \

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

TARGET_DIR = ../bin
TARGET = test

all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET_DIR)/$@ $(OBJS) $(LDFLAGS) $(LIBS)

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
