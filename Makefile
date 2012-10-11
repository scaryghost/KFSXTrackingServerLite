CPPC=g++
LIB_DIR=/media/hdd2/local/share/lib 
EXT_LIB_LIST=stoughton1004 \
    sqlite3
INC_DIR_LIST=. /media/hdd2/local/share/include

SRC_LIST=Time.cpp Utils.cpp Content.cpp
MAIN=main.cpp
EXE=kfsxtslite

CPP_FLAGS=-std=c++0x -Werror -Wall
INC_DIRS= $(foreach path, $(INC_DIR_LIST), -I$(path))
SRCS= $(foreach src, $(SRC_LIST), src/$(src))
OBJS= $(patsubst %.cpp, %.o, $(SRCS))
EXT_LIBS= $(foreach lib, $(EXT_LIB_LIST), -l$(lib))

all: $(OBJS)
	$(CPPC) $(CPP_FLAGS) $(INC_DIRS) -L$(LIB_DIR) -o $(EXE) $(MAIN) $(OBJS) $(EXT_LIBS)

%.o: %.cpp
	$(CPPC) -c $(CPP_FLAGS) $(INC_DIRS) $< -o $@

clean:
	rm -f $(OBJS) $(EXE)
