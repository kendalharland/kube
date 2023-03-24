
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

CXX := clang++
CXXFLAGS += -std=c++20 -I./src/includes

INC := -Llib
LIBS := -lglfw -lGLEW -lGL -lm
TARGET := kube

.PHONY: all clean help format

default: build

format:
	@echo "=== Formatting code ==="
	find src -regex '.*\.\(c\|h\|cpp\|hpp\)' -exec clang-format -style=file -i {} \;

assimp:
	@echo "=== Building third_party/assimp ==="
	cd third_party/assimp; cmake CMakeLists.txt; make -j4

dependencies: assimp
	@echo "=="

clean:
	@echo "=== Removing kube ==="
	rm -rf $(TARGET) ./*.o

build: dependencies format
	@echo "=== Building $(TARGET) ($(config)) ==="
	$(CXX) src/*.cpp -o $(TARGET) $(CXXFLAGS) $(LIBS) $(INC)

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
