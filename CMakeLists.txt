cmake_minimum_required(VERSION 3.28)
project(Pong C)

set(CMAKE_C_STANDARD 11)

add_executable(Pong main.c)

find_package(raylib REQUIRED)

target_link_libraries(${PROJECT_NAME} raylib)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()