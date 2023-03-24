
# To install dependencies:
# * libglfw: https://www.glfw.org/
# * libGLEW: https://glew.sourceforge.net/
# * libGL: https://dri.freedesktop.org/wiki/libGL/

ifndef config
	config = release
endif

ifeq ($(config),debug)
	CXXFLAGS += -g -O0
endif

SOURCE_ROOT := $(realpath ./)

# Ideally this is a directory in the project but for some
# reason clang++ can't find libraries unless its this specific
# directory and the user has run `export LIBRARY_PATH=/usr/local/lib`
# *shrug*.
LIBRARY_PATH=$(SOURCE_ROOT)/src/lib/
LIBS := -lglfw -lGLEW -lGL -lm -lassimp
INC := -I./src/include
INC += -I./third_party/assimp/include
ENV := LIBRARY_PATH=$(LIBRARY_PATH) LD_LIBRARY_PATH=$(LIBRARY_PATH)
CXX := clang++
CXXFLAGS += -std=c++20 $(LIBS) $(INC)

TARGET := kube

.PHONY: all clean help format

default: build

format:
	@echo "=== Formatting code ==="
	find src -regex '.*\.\(c\|h\|cpp\|hpp\)' -exec clang-format -style=file -i {} \;

assimp:
	@echo "=== Building third_party/assimp ==="
	cd third_party/assimp; cmake CMakeLists.txt; make -j4
	sudo cp -av third_party/assimp/bin/libassimp.so* $(LIBRARY_PATH)

dependencies: assimp
	@echo "=="

clean:
	@echo "=== Removing kube ==="
	rm -rf $(TARGET) ./*.o

build: dependencies format
	@echo "=== Building $(TARGET) ($(config)) ==="
	@echo "==="
	@echo "=== WARNING: If you see link errors, try running the following commands and rebuilding."
	@echo "==="
	@echo "===     export LD_LIBRARY_PATH=$(LIBRARY_PATH)"
	@echo "===     export LIBRARY_PATH=$(LIBRARY_PATH)"
	@echo "==="
	$(CXX) src/*.cpp -o $(TARGET) $(CXXFLAGS) 

clean-build: clean build
	@echo "=== Performing a clean build ==="

run: build
	@echo "=== Running $(TARGET) ($(config)) ==="
	./$(TARGET)

help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@echo "  kube (default)"
	@echo "  clean"
	@echo "  format"
	@echo "  run"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"
