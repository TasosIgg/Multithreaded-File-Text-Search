cmake_minimum_required(VERSION 3.10)
project(multigrep_gui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)   # enable Qt's MOC
set(CMAKE_AUTOUIC ON)   # enable Qt Designer UI (if used)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(multigrep
    main.cpp
    SearchWindow.cpp
    SearchManager.cpp
    ThreadPool.cpp
    SearchWorker.cpp
    FileUtils.cpp
)

target_link_libraries(multigrep PRIVATE Qt5::Widgets)
