ifndef config
	config = release
endif

ifeq ($(config),debug)
	CXXFLAGS += -g -O0
endif

CXX := clang++
CXXFLAGS += -std=c++20 -I./src/includes

LIBS := -lglfw -lGLEW -lGL -lm
TARGET := kube

.PHONY: all clean help

kube: clean
	@echo "=== Building $(TARGET) ($(config)) ==="
	$(CXX) src/*.cpp -o $(TARGET) $(CXXFLAGS) $(LIBS)

clean:
	@echo "=== Removing kube ==="
	rm -rf $(TARGET) ./*.o

format:
	@echo "=== Formatting code ==="
	find src -regex '.*\.\(c\|h\|cpp\|hpp\)' -exec clang-format -style=file -i {} \;

run: kube
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
