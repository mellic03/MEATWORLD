cmake_minimum_required(VERSION 3.9)
project(game)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)


# find_package(SDL2_net REQUIRED)
# include_directories(${SDL2_net_INCLUDE_DIRS})


file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/systems/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/menu/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/meatnet/*.cpp
)


add_library(${PROJECT_NAME} SHARED ${SOURCES})



target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

if (IDK_DEBUG)
    target_compile_options(${PROJECT_NAME} PUBLIC -O0 -g)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC -Ofast)
endif ()




target_include_directories(
    ${PROJECT_NAME} PUBLIC
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC

    libidk
    IDKGraphics

    IDKAudio
    IDKEvents
    IDKBuiltinCS
    IDKThreading
    IDKECS

    SDL2_net
)



FILE(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/
)

FILE(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders/
)

