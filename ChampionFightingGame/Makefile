CWD = $(shell cd)
TARGET=ChampionFightingGame.exe
OBJECT_NAMES=main.o Animation.o Box.o GameCoordinate.o Game.o ParamTable.o PlayerInfo.o utils.o Roy.o Eric.o Debugger.o
OBJECT_FILES=$(patsubst %,$(CWD)/%,$(OBJECT_NAMES))
INCLUDE_DIR=../SDL2/include
LIB_DIR=../SDL2/lib/x86
LIB_FILES=-lSDL2 -lSDL2main -llibpng16-16 -lSDL2_image
OBJ_FILE_DIR=Debug/
CFLAGS=-I $(INCLUDE_DIR) -L $(LIB_DIR) -static-libgcc -static-libstdc++


all: $(CWD)/$(TARGET)

$(CWD)/$(TARGET): $(OBJECT_FILES)
	g++ $(OBJECT_FILES) $(CFLAGS) $(LIB_FILES) -o $(TARGET)

%.o: %.cpp
		g++ -c $(CFLAGS) -o $@ $^

clean:
	del *.o 
	del $(TARGET)	