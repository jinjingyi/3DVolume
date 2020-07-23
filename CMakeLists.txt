cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(APP_NAME "3DVolume")
set(APP_ICON ${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/3DVolume.rc)

set(CMAKE_CONFIGURATION_TYPES Release)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

PROJECT(3DVolume)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(Qt5Core REQUIRED QUIET)
find_package(Qt5Gui REQUIRED QUIET)
find_package(Qt5LinguistTools REQUIRED QUIET)
find_package(Qt5Widgets REQUIRED QUIET)

set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREAD ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")

find_package(Boost COMPONENTS system filesystem REQUIRED )

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
message(STATUS "${Boost_INCLUDE_DIRS}")
message(STATUS "${Boost_LIBRARY_DIRS}")

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
message(STATUS "${OpenCV_INCLUDE_DIRS}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE UI_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.ui)
file(GLOB_RECURSE QRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/assets/*.qrc)

add_executable(${APP_NAME} WIN32 MACOSX_BUNDLE ${APP_ICON} ${UI_FILES} ${QRC_FILES} ${HEADER_FILES} ${CPP_FILES})
target_link_libraries(${APP_NAME} Qt5::Core Qt5::Gui Qt5::Widgets ${GLUE} ${VTK_LIBRARIES} ${Boost_LIBRARIES} ${OpenCV_LIBS})

