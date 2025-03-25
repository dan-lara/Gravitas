# Variables
BUILD_DIR = build
EXECUTABLE = $(BUILD_DIR)/bin/main
SOURCE_FILE ?= src/main.cpp

# Default target
all: build run

# Build the project using CMake
build:
	cmake -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

# Run the executable
run:
	@echo "Running the executable..."
	$(EXECUTABLE)

# Clean the build directory
clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR)

.PHONY: all build run clean