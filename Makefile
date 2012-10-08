CPPC=g++
LIB_DIR=/media/hdd2/local/share/lib 
EXT_LIB_LIST=stoughton1004 \
    sqlite3
INC_DIRS=/media/hdd2/local/share/include

SRCS=
MAIN=main.cpp
EXE=kfsxtslite

CPP_FLAGS=-std=c++0x -Werror -Wall -I$(INC_DIRS)
OBJS= $(patsubst %.cpp, %.o, $(SRCS))
EXT_LIBS= $(foreach lib, $(EXT_LIB_LIST), -l$(lib))

all: $(OBJS)
	$(CPPC) $(CPP_FLAGS) -o $(EXE) $(MAIN) $(OBJS) -L$(LIB_DIR) $(EXT_LIBS)

%.o: %.cpp
	$(CPPC) -c $(CPP_FLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(EXE)
