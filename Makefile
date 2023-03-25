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

LIBRARY_PATH=$(SOURCE_ROOT)/src/lib/
LIBS := -lglfw -lGLEW -lGL -lm -lassimp
INC := -I./src/include
INC += -I./third_party/assimp/include
CXX := clang++
CXXFLAGS += -std=c++20 -Wall $(LIBS) $(INC)

TARGET := $(LIBRARY_PATH)libkube.o

.PHONY: all clean help format

default: demo

format:
	@echo "=== Formatting code ==="
	find src -regex '.*\.\(c\|h\|cpp\|hpp\)' -exec clang-format -style=file -i {} \;

assimp:
	@echo "=== Building third_party/assimp ==="
	cd third_party/assimp; cmake CMakeLists.txt; make -j4
	cp -av third_party/assimp/bin/libassimp.so* $(LIBRARY_PATH)

dependencies: assimp
	@echo "=="

clean:
	@echo "=== Removing kube ==="
	rm -rf $(TARGET) ./*.o

build: dependencies format
	@echo "=== Building $(TARGET) ($(config)) ==="
	$(CXX) -c -o $(TARGET) src/kube.cpp $(CXXFLAGS) -fPIC
	$(CXX) -shared -o $(LIBRARY_PATH)libkube.so $(TARGET)


clean-build: clean build
	@echo "=== Performing a clean build ==="

demo: build
	@echo "=== Running $(TARGET) ($(config)) ==="
	$(CXX) demos/rotating_cube/main.cpp -o bin/rotating_cube $(CXXFLAGS) -lkube

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
