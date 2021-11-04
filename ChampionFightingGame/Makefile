CWD = $(shell cd)
TARGET=ChampionFightingGame.exe
OBJECT_NAMES=main.o Animation.o Box.o GameCoordinate.o Game.o ParamTable.o PlayerInfo.o utils.o Roy.o Eric.o Debugger.o UI.o Stage.o Menu.o FighterInstance.o DebugMenu.o FighterInstanceAccessor.o ObjectInstance.o ProjectileInstance.o RoyFireball.o EricFireball.o CharaTemplate.o ProjectileTemplate.o Atlas.o MenuHandler.o
OBJECT_FILES=$(patsubst %,$(CWD)/%,$(OBJECT_NAMES))
INCLUDE_DIR=../SDL2/include
LIB_DIR=../SDL2/lib/x86
LIB_FILES=-lSDL2 -lSDL2main -llibpng16-16 -lSDL2_image -lSDL2_ttf -llibtiff-5
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
	del prebuild\*

package: all
	copy /y /v ..\sdl2\lib\x86\*.dll prebuild
	xcopy resource prebuild\resource /I /E /H /C /Y 
	copy /y /v ChampionFightingGame.exe prebuild
	7z a -tzip prebuild.zip prebuild