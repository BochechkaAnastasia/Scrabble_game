cmake_minimum_required(VERSION 3.16)

cmake_minimum_required(VERSION 3.16)
project(ScrabbleGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    main.cpp
    mainwindow.cpp
    scrabblegame.cpp
)

set(HEADERS
    mainwindow.h
    scrabblegame.h
)

add_executable(ScrabbleGame ${SOURCES} ${HEADERS}
    doctest.h)
target_link_libraries(ScrabbleGame Qt6::Widgets)

enable_testing()

add_executable(tests test.cpp scrabblegame.cpp)
target_link_libraries(tests Qt6::Widgets)

add_test(NAME ScrabbleTests COMMAND tests)
