set(QT_PATH /opt/Qt5.6.2/5.6/gcc_64/lib/cmake/)

cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

project(bitviewer)

set (CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)
file(GLOB UI *.ui)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets PATHS ${QT_PATH} REQUIRED)
find_package(Qt5DBus PATHS ${QT_PATH} REQUIRED)

#add_executable(main WIN32 ${SOURCES} ${HEADERS} ${UI}) # that's one for Windows if you want to hide the console
add_executable(main ${SOURCES} ${HEADERS} ${UI})
target_link_libraries(main Qt5::Widgets Qt5::DBus Qt5::Gui Qt5::Core)

