cmake_minimum_required(VERSION 3.12)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 11)


set(SOURCES
    main.cpp
    game.h
    game.cpp
)


set(SFML_DIR "/lib/SFML-2.6.0/lib/cmake")
find_package(SFML 2.6 COMPONENTS graphics audio REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})


target_include_directories(${PROJECT_NAME} PRIVATE "/lib/SFML-2.6.0/include")


target_link_directories(${PROJECT_NAME} PRIVATE "/lib/SFML-2.6.0/lib")


target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-audio)
