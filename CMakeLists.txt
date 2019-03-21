cmake_minimum_required (VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl -lpthread -Wall -fPIC")
project(app)
option(QT5BUILD "Build using Qt5 libs" TRUE)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET( QT_WRAP_CPP TRUE)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core)
find_package(Qt5Network)
find_package(Qt5Multimedia)
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS} ${Qt5Multimedia_INCLUDE_DIRS})
include_directories(lib/StanfordCPPLib lib/StanfordCPPLib/io lib/StanfordCPPLib/collections lib/StanfordCPPLib/system lib/StanfordCPPLib/private lib/StanfordCPPLib/util lib/StanfordCPPLib/graphics)
file(GLOB stanford_files
        "lib/StanfordCPPLib/graphics/*.h"
        "lib/StanfordCPPLib/graphics/*.cpp"
        "lib/StanfordCPPLib/util/*.h"
        "lib/StanfordCPPLib/util/*.cpp"
        "lib/StanfordCPPLib/system/*.h"
        "lib/StanfordCPPLib/system/*.cpp"
        "lib/StanfordCPPLib/collections/*.h"
        "lib/StanfordCPPLib/collections/*.cpp"
        "lib/StanfordCPPLib/private/*.h"
        "lib/StanfordCPPLib/private/*.cpp"
        "lib/StanfordCPPLib/io/*.h"
        "lib/StanfordCPPLib/io/*.cpp"
        )
add_library(stanford ${stanford_files})
IF (QT5BUILD)
    find_package(Qt5Core REQUIRED)
    set(QT_INCLUDES ${Qt5Core_INCLUDE_DIRS})
    include_directories(${QT_INCLUDES})
    QT5_WRAP_CPP(app_HEADERS_MOC ${HuggleLite_HEADERS})
ELSE()
    find_package(Qt4 REQUIRED)
ENDIF()
set(app_SOURCES src/hello.cpp)
file(GLOB g_files
        "lib/StanfordCPPLib/graphics/*.cpp"
        )
qt5_wrap_cpp(moc_sources ${g_files})
link_libraries(stanford Qt5::Widgets Qt5::Network Qt5::Core Qt5::Multimedia)
add_executable(app ${app_SOURCES})
IF (QT5BUILD)
    qt5_use_modules(app Core)
ENDIF()
# qt_wrap_cpp(Qt5Core lib/StanfordCPPLib/graphics/gdownloader.cpp)
file(COPY lib/spl.jar
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
qt5_wrap_cpp(moc_sources lib/)
qt5_wrap_ui(uic_sources src/MainWindow.cpp)

#target_link_libraries(app stanford Qt5::Widgets Qt5::Network Qt5::Core Qt5::Multimedia)
