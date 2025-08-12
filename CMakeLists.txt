cmake_minimum_required(VERSION 3.29)
project(Phase2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(WIN32)
    set(QT_DIR "C:/Qt/6.7.3/mingw_64/bin")
    set(ENV{PATH} "${QT_DIR};$ENV{PATH}")
endif()
# مسیر ساده برای خروجی (همه فایل‌ها در bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "C:/QtProjects/Phase2/bin")

# Qt automoc/auto uic/autorcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# مسیر windeployqt.exe
set(WINDEPLOYQT_EXE "C:/Qt/6.7.3/mingw_64/bin/windeployqt.exe" CACHE FILEPATH "Path to windeployqt.exe.")

# مسیر سورس
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# include paths
include_directories(
        "${SRC_DIR}/view/components"
        "${SRC_DIR}/view/wire"
        "${SRC_DIR}/view/ui"
        "${SRC_DIR}/view/grid"
        "${SRC_DIR}/model/Circuit Model"
        "${SRC_DIR}/model/Circuit Model/components"
        "${SRC_DIR}/model/Results"
        "${SRC_DIR}/model/Solver"
        "${SRC_DIR}/controller/Supporting Controllers"
        "${SRC_DIR}/controller/Core Controllers"
        "C:/Program Files/eigen-3.4.0/eigen-3.4.0"
)

# سورس‌ها
file(GLOB_RECURSE SOURCES
        "${SRC_DIR}/main.cpp"
        "${SRC_DIR}/view/components/*.cpp"
        "${SRC_DIR}/view/wire/*.cpp"
        "${SRC_DIR}/view/ui/*.cpp"
        "${SRC_DIR}/view/grid/*.cpp"
        "${SRC_DIR}/controller/Core Controllers/*.cpp"
        "${SRC_DIR}/controller/Supporting Controllers/*.cpp"
        "${SRC_DIR}/model/Circuit Model/components/*.cpp"
        "${SRC_DIR}/model/Circuit Model/*.cpp"
        "${SRC_DIR}/model/Circuit Mode/components/*.cpp"
        "${SRC_DIR}/model/Circuit Mode/*.cpp"
        "${SRC_DIR}/model/Results/*.cpp"
        "${SRC_DIR}/model/Solver/*.cpp"
)

# پیدا کردن Qt
find_package(Qt6 COMPONENTS Core Gui Widgets Charts PrintSupport REQUIRED)

# executable
add_executable(${PROJECT_NAME} ${SOURCES})

# لینک Qt
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Charts
        Qt6::PrintSupport
)

# رفع مشکل too many sections برای qcustomplot.cpp
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(QCUSTOM_PLOT_SRC "${SRC_DIR}/model/Results/qcustomplot.cpp")
    if (EXISTS "${QCUSTOM_PLOT_SRC}")
        set_source_files_properties(
                "${QCUSTOM_PLOT_SRC}"
                PROPERTIES COMPILE_FLAGS "-Wa,-mbig-obj"
        )
    endif()
endif()

# اجرای windeployqt بعد از بیلد
if (WIN32 AND WINDEPLOYQT_EXE)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${WINDEPLOYQT_EXE}" --no-translations "$<TARGET_FILE:${PROJECT_NAME}>"
            WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            COMMENT "Running windeployqt to copy Qt DLLs..."
            VERBATIM
    )
endif()
