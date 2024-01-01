cmake_minimum_required(VERSION 3.9)

set(CMAKE_BUILD_PARALLEL_LEVEL 8)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wuninitialized -Winit-self -Wmaybe-uninitialized -Werror -fsanitize=undefined -fdiagnostics-color=always -lstdc++fs")
add_compile_options("-fPIC" "-shared")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/output)


# Debugging
# --------------------------------------------
add_definitions(-DIDK_DEBUG)
# --------------------------------------------
project(game)



include_directories(${CMAKE_SOURCE_DIR}/external/include/)
include_directories(${CMAKE_SOURCE_DIR}/IDKGame/src/)



add_subdirectory(external/sources/imgui-1.90/)
add_subdirectory(external/sources/imgui-docking/)
add_subdirectory(IDKGame/src/ComponentSystems/)
add_subdirectory(IDKGame/src/Modules/)



file(GLOB SOURCES
    "IDKGame/src/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

# set_target_properties(${PROJECT_NAME} PROPERTIES
#     VERSION        "${PROJECT_VERSION}"
#     SOVERSION      "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
# )

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    # ${CMAKE_SOURCE_DIR}/external/lib/libIDKengine.so
    # ImGui_190
    ImGui_docking
    IDKcomponentsystems
    IDKmodules
)

