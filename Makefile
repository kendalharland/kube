ifndef config
		config = release
endif

ifeq ($(config),debug)
endif

CXX := clang++
CXXFLAGS += -std=c++20 -L/usr/local/lib -L/usr/lib -L/usr/lib65

LIBS := -lglfw -lGLEW -lGL -lm -lpthread -ldl -lrt -lX11 
TARGET := kube

.PHONY: all clean help

clean:
	@echo "=== Removing kube ==="
	rm -rf $(TARGET) ./*.o

format:
	@echo "=== Formatting code ==="
	find src -regex '.*\.\(c\|h\|cpp\|hpp\)' -exec clang-format -style=file -i {} \;

kube: clean
	@echo "=== Building $(TARGET) ($(config)) ==="
	$(CXX) src/main.cpp -o $(TARGET) $(CXXFLAGS) $(LIBS)

run: kube
	@echo "=== Running $(TARGET) ($(config)) ==="
	./$(TARGET)

help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@echo "  all (default)"
	@echo "  clean"
	@echo "  kube"
	@echo "  run"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"

