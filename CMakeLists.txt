cmake_minimum_required(VERSION 3.31)
project(ProductionAnalysis)

set(CMAKE_CXX_STANDARD 26)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets LinguistTools Sql)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*.hpp")

qt_standard_project_setup()
qt_add_executable(ProductionAnalysis ${SOURCES} ${HEADERS})
qt_add_translations(ProductionAnalysis
    TS_FILES
        "translations/ProductionAnalysis_en_US.ts"
        "translations/ProductionAnalysis_ru_RU.ts"
)
target_include_directories(ProductionAnalysis PRIVATE include)
target_link_libraries(ProductionAnalysis PRIVATE Qt::Core Qt::Gui Qt::Widgets Qt::Sql)
