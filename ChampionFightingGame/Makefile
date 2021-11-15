CWD = $(shell cd)
TARGET=ChampionFightingGame.exe
OBJECT_NAMES=main.o Animation.o Box.o GameCoordinate.o Game.o ParamTable.o PlayerInfo.o utils.o Roy.o Eric.o Debugger.o UI.o Stage.o Menu.o Fighter.o DebugMenu.o FighterAccessor.o Object.o Projectile.o RoyFireball.o EricFireball.o CharaTemplate.o ProjectileTemplate.o Atlas.o MenuHandler.o CharaSelect.o GameTexture.o SoundManager.o Angelica.o Nightsaber.o Sully.o Priest.o Aziel.o Bruno.o Tessa.o Alejandro.o Norman.o Julius.o Ramona.o Zyair.o Vesuvius.o Leon.o Loader.o TitleScreen.o FighterStatus.o
OBJECT_FILES=$(patsubst %,$(CWD)/%,$(OBJECT_NAMES))
INCLUDE_DIR=-I ../SDL2/include -I C:/MinGW/include/
LIB_DIR=-L ../SDL2/lib/x64 -L C:/MinGW/lib/
LIB_FILES=-lSDL2 -lSDL2main -llibpng16-16 -lSDL2_image -lSDL2_ttf -llibtiff-5
OBJ_FILE_DIR=Debug/
CFLAGS= $(INCLUDE_DIR) $(LIB_DIR) -static-libgcc -static-libstdc++


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

launch: all
	$(CWD)/$(TARGET)