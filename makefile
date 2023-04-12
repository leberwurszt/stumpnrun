BINARY_NAME = stumpnrun

COMPILER = g++
COMPILER_FLAGS =
COMPILER_FLAGS += -std=c++17
COMPILER_FLAGS += -Werror

HEADER_DIR = ./cpp
SOURCE_DIR = ./cpp
BUILD_DIR = ./build

LIBS =
LIBS += -lSDL2
LIBS += -lSDL2_image
LIBS += -lSDL2_ttf
LIBS += -lSDL2_mixer

SRC_FILES =
SRC_FILES += $(SOURCE_DIR)/main.cpp
SRC_FILES += $(SOURCE_DIR)/engine.cpp
SRC_FILES += $(SOURCE_DIR)/level.cpp
SRC_FILES += $(SOURCE_DIR)/entity.cpp
SRC_FILES += $(SOURCE_DIR)/player.cpp

HDR_FILES =
HDR_FILES += $(SOURCE_DIR)/main.h
HDR_FILES += $(SOURCE_DIR)/engine.h
HDR_FILES += $(SOURCE_DIR)/level.h
HDR_FILES += $(SOURCE_DIR)/entity.h
HDR_FILES += $(SOURCE_DIR)/player.h

all: $(SRC_FILES) $(HDR_FILES)
	@$(COMPILER) -o $(BUILD_DIR)/$(BINARY_NAME) $(SRC_FILES) $(LIBS)
	@cp -r ./tileset ./build
	@cp -r ./level ./build
	@echo Done.

clean:
	@rm -r $(BUILD_DIR)/*