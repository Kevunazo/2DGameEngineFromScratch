#########################################################
# Declare Makefile Vars
#########################################################
CCompiler = g++
COMPILER_FLAGS = -Wall -Wfatal-errors
LANG_STD = -std=c++17
INCLUDE_PATH = -I"./libs/"
SORUCE_FILES = src/*.cpp \
			   src/Game/*.cpp \
			   src/Logger/*.cpp \
			   src/ECS/*.cpp \
			   src/Components/*.h \


LINKER_FLAGS = -lSDL2 \
			   -lSDL2_image \
			   -lSDL2_mixer \
			   -lSDL2_ttf \
			   -llua5.3
OBJ_NAME = gameengine

#########################################################
# Declare Makefile Rules
#########################################################

build:
	$(CCompiler) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH)  $(SORUCE_FILES) $(LINKER_FLAGS) -o $(OBJ_NAME);

run:
	./gameengine

clean:
	rm gameengine
