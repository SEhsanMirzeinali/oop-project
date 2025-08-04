cmake_minimum_required(VERSION 3.29)
project(Phase2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# اضافه کردن پوشه‌ها
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/view/components
        ${CMAKE_CURRENT_SOURCE_DIR}/view/wire
        ${CMAKE_CURRENT_SOURCE_DIR}/view/ui
        ${CMAKE_CURRENT_SOURCE_DIR}/view/grid
        ${CMAKE_CURRENT_SOURCE_DIR}/model/Circuit Model
        ${CMAKE_CURRENT_SOURCE_DIR}/model/Circuit Model/components
        ${CMAKE_CURRENT_SOURCE_DIR}/model/Results
        ${CMAKE_CURRENT_SOURCE_DIR}/model/Solver
        ${CMAKE_CURRENT_SOURCE_DIR}/controller/Supporting Controllers
        ${CMAKE_CURRENT_SOURCE_DIR}/controller/Core Controllers
        "C:/Program Files/eigen-3.4.0/eigen-3.4.0"


)
#include_directories("C:/Program Files/eigen-3.4.0/eigen-3.4.0")

# فایل‌ها
file(GLOB_RECURSE SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/view/components/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/view/wire/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/view/ui/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/view/grid/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/controller/Core Controllers/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/controller/Supporting Controllers/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model/Circuit Mode/components/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model/Circuit Mode/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model/Results/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model/Solver/*.cpp




)

find_package(Qt6 COMPONENTS Core Gui Widgets Charts PrintSupport REQUIRED)


add_executable(${PROJECT_NAME} ${SOURCES}
        view/ui/MainWindow.cpp
        view/ui/MainWindow.h
        view/ui/ComponentPropertyDialog.cpp
        view/components/GND.cpp
        view/ui/NetListHandler.h
        view/components/InductorComponent.cpp
        view/components/CapacitorComponent.cpp
        view/components/CapacitorComponent.h
        view/ui/CommandHandler.cpp
        "controller/Core Controllers/CircuitController.cpp"
        "controller/Supporting Controllers/IOController.cpp"
        "controller/Core Controllers/ResultController.h"
        "controller/Core Controllers/SimulationController.h"
        "controller/Supporting Controllers/SettingsController.h"
        "controller/Supporting Controllers/UserSessionController.h"
        "model/Circuit Model/CircuitModel.cpp"
        view/ui/SimulationDialog.cpp
        view/ui/SimulationDialog.cpp
        model/Results/Diagrom.cpp
        model/Results/Diagrom.h
        model/Results/qcustomplot.cpp
        #"controller/Supporting Controllers/IOController.cpp"
        #"model/Circuit Model/CircuitModel.cpp"
)

target_link_libraries(${PROJECT_NAME}
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt6::Charts
        Qt6::PrintSupport

)

# Windows DLL handling (مطابق قبل)
