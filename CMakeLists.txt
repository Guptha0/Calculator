cmake_minimum_required(VERSION 3.16)
project(CalculatorPro VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(COMMON_HEADERS
    include/common/Exceptions.hpp
)

set(ENGINE_HEADERS
    include/engine/ICalculatorMode.hpp
    include/engine/ExpressionEvaluator.hpp
    include/engine/StandardMode.hpp
    include/engine/ScientificMode.hpp
    include/engine/ProgrammerMode.hpp
    include/engine/ProgrammerEngine.hpp
    include/engine/StatisticsEngine.hpp
    include/engine/MemoryRegister.hpp
    include/engine/Calculator.hpp
)

set(STORAGE_HEADERS
    include/storage/HistoryManager.hpp
)

set(ENGINE_SOURCES
    src/engine/ExpressionEvaluator.cpp
    src/engine/StandardMode.cpp
    src/engine/ScientificMode.cpp
    src/engine/ProgrammerMode.cpp
    src/engine/ProgrammerEngine.cpp
    src/engine/StatisticsEngine.cpp
    src/engine/Calculator.cpp
    src/storage/HistoryManager.cpp
)

set(UI_HEADERS
    include/ui/MainWindow.hpp
)

set(UI_SOURCES
    src/ui/MainWindow.cpp
)

add_executable(CalculatorGUI
    src/main.cpp
    ${COMMON_HEADERS}
    ${ENGINE_HEADERS}
    ${STORAGE_HEADERS}
    ${ENGINE_SOURCES}
    ${UI_HEADERS}
    ${UI_SOURCES}
    resources/resources.qrc
)

target_include_directories(CalculatorGUI PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(CalculatorGUI PRIVATE Qt6::Widgets)
