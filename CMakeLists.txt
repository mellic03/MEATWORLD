cmake_minimum_required(VERSION 3.9)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_BUILD_PARALLEL_LEVEL 8)
# add_compile_options( -pedantic -Wextra )
set(CMAKE_CXX_FLAGS "-lstdc++fs -std=c++17 -fsanitize=undefined -g -fdiagnostics-color=always")

add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)


# Debugging
# --------------------------------------------
add_definitions(-DIDK_DEBUG)
# --------------------------------------------

# IDKEngine source directory
# --------------------------------------------
set(idkengine_source_dir "IDKGameEngine/src/")
add_subdirectory(IDKGameEngine/src/)
# --------------------------------------------

add_subdirectory(IDKGame/src/ComponentSystems/)
add_subdirectory(IDKGame/src/modules/)

project(game)

add_executable(
    ${PROJECT_NAME} "IDKGame/src/main.cpp"
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKengine
    IDKcomponentsystems
    IDKmodules
)






project(texconvert)

add_executable(
    ${PROJECT_NAME} "IDKGameEngine/src/IDKtools/texconvert.cpp"
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    IDKengine
    IDKcomponentsystems
    IDKmodules
)
