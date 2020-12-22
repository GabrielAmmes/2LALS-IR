#definitions
DIR_INC = ./src
DIR_ABC_LIB = ./
DIR_ABC_INC = ./
DIR_SRC = ./src
DIR_OBJ = ./obj
SOURCE  := $(wildcard ${DIR_SRC}/*.c) $(wildcard ${DIR_SRC}/*.cpp)
OBJS    := $(patsubst ${DIR_SRC}/%.c,${DIR_OBJ}/%.o,$(patsubst ${DIR_SRC}/%.cpp,${DIR_OBJ}/%.o,$(SOURCE)))
TARGET  := espresso
TARGET_L:= libespresso.a

#compiling parameters
CC      := gcc
LIBS    :=
LDFLAGS := -L ${DIR_ABC_LIB}
DEFINES := $(FLAG)
INCLUDE := -I ${DIR_INC} -I ${DIR_ABC_INC}
CFLAGS  := -g -Wall -O3 $(DEFINES) $(INCLUDE)
CXXFLAGS:= $(CFLAGS)

#commands
.PHONY : all lib objs rebuild clean init ctag
all : ctag init $(TARGET)

lib : ctag init $(TARGET_L)

objs : init $(OBJS)

rebuild : clean all

clean :
	rm -rf $(DIR_OBJ)
	rm -f $(TARGET)
	rm -f $(TARGET_L)
	rm -f tags

init :
	if [ ! -d obj ]; then mkdir obj; fi

ctag :
	ctags -R

$(TARGET) : $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

$(TARGET_L) : $(OBJS)
	ar cr $@ $(OBJS)

${DIR_OBJ}/%.o:${DIR_SRC}/%.c
	$(CC) $(CXXFLAGS) -c $< -o $@
