# Kube  :  An OpenGL based game engine.
# Copyright (C) 2021 Kendal Harland GPL-3.0-only
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

# To install dependencies:
# * libglfw: https://www.glfw.org/
# * libGLEW: https://glew.sourceforge.net/
# * libGL:   https://dri.freedesktop.org/wiki/libGL/

ifndef config
	config = release
endif

SOURCE_ROOT  := $(realpath ./)
LIBRARY_PATH := $(SOURCE_ROOT)/src/lib/
BIN_PATH     := $(SOURCE_ROOT)/bin

CXX      := clang++
CXXFLAGS += -std=c++20 -Wall
INC      := -I./src/include -I./third_party/assimp/include -I./third_party/wren/src/include

ifeq ($(config),debug)
	CXXFLAGS += -g -O0
else
	CXXFLAGS += -O2
endif

# Detect OS (Darwin = macOS, otherwise assume Linux/Unix-like)
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	SHLIB_EXT  := dylib
	LINKSHARED := -dynamiclib
	CXXFLAGS  += -I/opt/homebrew/include -I/opt/homebrew/opt/llvm/include
	LDFLAGS   += -L/opt/homebrew/lib -L$(SOURCE_ROOT)/third_party/assimp/bin \
							 -L$(SOURCE_ROOT)/third_party/wren/bin \
	             -L$(LIBRARY_PATH) -Wl,-rpath,@executable_path/../src/lib
	LIBS      := -lglfw -lGLEW -lassimp -lwren -lm -framework OpenGL
else
	SHLIB_EXT  := so
	LINKSHARED := -shared
	LDFLAGS   += -L$(LIBRARY_PATH)
	LIBS      := -lglfw -lGLEW -lGL -lm -lassimp -lwren
endif

TARGET_OBJ   := $(LIBRARY_PATH)libkube.o
TARGET_LIB   := $(LIBRARY_PATH)libkube.$(SHLIB_EXT)

.PHONY: default pull-submodules clean help format build build3p assimp wren demo demo-creatures demo-car demo-wren clean-build

default: demo

format:
	@echo "=== Formatting code ==="
	# find src -type f -exec clang-format -style=file -i {} \;

assimp:
	@echo "=== Building third_party/assimp ==="
	cd third_party/assimp; cmake CMakeLists.txt -DASSIMP_BUILD_TESTS=OFF -DASSIMP_BUILD_ASSIMP_TOOLS=OFF; make -j4
	@mkdir -p $(LIBRARY_PATH)
	# Copy the real dylib and its symlinks
	cp -av third_party/assimp/bin/libassimp*.dylib $(LIBRARY_PATH)

wren:
	@echo "=== Building third_party/wren ==="
	cd third_party/wren/projects/make.mac; make -j4
	@mkdir -p $(LIBRARY_PATH)
	# Copy the real dylib and its symlinks
	cp -av third_party/wren/lib/libwren*.dylib $(LIBRARY_PATH)

build3p: assimp wren
	@echo "=="

clean:
	@echo "=== Removing temporary files ==="
	rm -f $(TARGET_OBJ) $(TARGET_LIB)
	rm -f $(BIN_PATH)/*

$(TARGET_OBJ): src/kube.cpp
	@mkdir -p $(LIBRARY_PATH)
	@echo "=== Compiling libkube object ($(config)) ==="
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(INC) -fPIC

$(TARGET_LIB): $(TARGET_OBJ)
	@echo "=== Linking $(TARGET_LIB) ($(config)) ==="
	$(CXX) $(LINKSHARED) -o $@ $^ $(LDFLAGS) $(LIBS)

build: build3p format $(TARGET_LIB)
	@echo "=== Build complete: $(TARGET_LIB) ==="

clean-build: clean build
	@echo "=== Performing a clean build ==="

pull-submodules:
	@echo "=== Fetching submodules ==="
	git submodule update --init --recursive

demo: build
	@mkdir -p $(BIN_PATH)
	@echo "=== Building demos ($(config)) ==="
	# Ensure the linker finds libkube in src/lib
	$(CXX) demos/car/main.cpp       -o $(BIN_PATH)/car       $(CXXFLAGS) $(INC) -L$(LIBRARY_PATH) -lkube $(LDFLAGS) $(LIBS) -Idemos/common/
	$(CXX) demos/creatures/main.cpp -o $(BIN_PATH)/creatures $(CXXFLAGS) $(INC) -L$(LIBRARY_PATH) -lkube $(LDFLAGS) $(LIBS) -Idemos/common/
	$(CXX) demos/wren/main.cpp      -o $(BIN_PATH)/wren      $(CXXFLAGS) $(INC) -L$(LIBRARY_PATH) -lkube $(LDFLAGS) $(LIBS) -Idemos/common/

demo-creatures: demo
	./bin/creatures

demo-car: demo
	./bin/car

demo-wren: demo
	./bin/wren

help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@echo "  demo (default)   Builds all demos and places them in bin/"
	@echo "  clean            Deletes temporary files from the project directory"
	@echo "  format           Runs clang-format on all source files"
	@echo "  build            Builds libkube shared library"
	@echo "  clean-build      Cleans and then builds"
	@echo "  pull-submodules  Initializes/updates git submodules"

