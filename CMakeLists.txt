# CMakeList.txt : CMake project for Penmanship, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Penmanship")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_DEBUG_POSTFIX "d")

find_package(Qt5 COMPONENTS Core Widgets PrintSupport REQUIRED)

set(Qt5Libs Qt5::Core Qt5::Widgets Qt5::PrintSupport)
set(INCLUDES_DIRS
    "${PROJECT_BINARY_DIR}"
)

set(HDRS 
    src/Penmanship.h
    src/MainWindow.h
    src/EditGraphicsViewer.h
    src/EditGraphicsScene.h
    src/CellItem.h
)

set(SRCS
    src/Penmanship.cpp
    src/MainWindow.cpp
    src/EditGraphicsViewer.cpp
    src/EditGraphicsScene.cpp
    src/CellItem.cpp
)

set(UIS
)

set(CODES ${SRCS} ${HDRS} ${UIS})

# Add source to this project's executable.
add_executable (${PROJECT_NAME} ${CODES})
target_include_directories(${PROJECT_NAME} PUBLIC "${INCLUDES_DIRS}")
target_link_libraries(${PROJECT_NAME} ${Qt5Libs})
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# TODO: Add tests and install targets if needed.
