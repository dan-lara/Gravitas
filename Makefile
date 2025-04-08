# Variables
BUILD_DIR = build
EXECUTABLE = $(BUILD_DIR)/bin/
SOURCE_FILE ?= src/main.cpp

# Default target
all: build

rebuild: clean build run

# Build the project using CMake
build:
	cmake -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

# Run the executable
run: build
	@echo "Running the executable..."
	$(EXECUTABLE)main

# Run examples
examples:
	@echo "Available examples:"
	@ls $(BUILD_DIR)/bin | grep -v main
	@read -p "Enter the example file to run: " EXAMPLE_FILE; \
	$(BUILD_DIR)/bin/$$EXAMPLE_FILE

# Clean the build directory
clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR)

# Display help information
help:
	@echo "Available targets:"
	@echo "  all       - Build and run the project"
	@echo "  rebuild   - Clean, build, and run the project"
	@echo "  build     - Build the project using CMake"
	@echo "  run       - Run the executable"
	@echo "  examples  - List and run example executables"
	@echo "  clean     - Clean the build directory"
	@echo "  help      - Display this help message"

.PHONY: all build run clean examples help