cmake_minimum_required(VERSION 3.15.0)

project(SPConfigurator VERSION 1.0.0 LANGUAGES CXX)
set(BIN_NAME SPConfigurator) 

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Populate a CMake variable with the sources
set(SPConfigurator_SRCS
    mainwindow.ui
    mainwindow.h
    mainwindow.cpp
    SPConfigurator.cpp
    SPConfigurator.h
    SPConfiguratorListener.h
    main.cpp
)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

add_executable(${BIN_NAME} ${SPConfigurator_SRCS})

set_target_properties(${BIN_NAME} PROPERTIES CXX_STANDARD 17)
set_target_properties(${BIN_NAME} PROPERTIES LINK_FLAGS -pthread)

# Find includes in corresponding build directories
# set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(g3logger REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

# Find the Qt libraries
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)

target_include_directories(${BIN_NAME} PUBLIC ${Boost_INCLUDE_DIRS} ${G3LOG_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS})

target_link_libraries(${BIN_NAME} ${Boost_LIBRARIES} ${G3LOG_LIBRARIES})
target_link_libraries(${BIN_NAME} ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES})
