cmake_minimum_required(VERSION 3.31)
project(ProductionAnalysis)

set(CMAKE_CXX_STANDARD 26)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*.hpp")

qt_standard_project_setup()
qt_add_executable(ProductionAnalysis ${SOURCES} ${HEADERS})
target_include_directories(ProductionAnalysis PRIVATE include)
target_link_libraries(ProductionAnalysis PRIVATE stdc++ Qt::Core Qt::Gui Qt::Widgets Qt::Sql)
