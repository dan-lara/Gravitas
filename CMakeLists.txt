cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

include_directories(src/libs src/assets)
file(GLOB_RECURSE LIB_SOURCES src/libs/*.cpp)
file(GLOB_RECURSE LIB_HEADERS src/libs/*.hpp)
file(GLOB_RECURSE ASSET_FILES src/assets/*)

# Define a macro for the assets directory
add_compile_definitions(ASSETS_DIR="${CMAKE_SOURCE_DIR}/src/assets/")

# Main executable
add_executable(main src/main.cpp ${LIB_SOURCES})
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)

# Examples
file(GLOB EXAMPLES_SOURCES src/examples/*.cpp)

foreach(EXAMPLE ${EXAMPLES_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE} ${LIB_SOURCES})
    target_compile_features(${EXAMPLE_NAME} PRIVATE cxx_std_17)
    target_link_libraries(${EXAMPLE_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)
endforeach()

