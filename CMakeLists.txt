#-----------------------------------------------------------------
# Project converted from qmake file using q2c
# https://github.com/benapetr/q2c at Thu Mar 21 00:48:14 2019
#-----------------------------------------------------------------
cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_STANDARD 17)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl -lpthread")
add_compile_options(-lLLVMSupport -ldl)
project(app)
option(QT5BUILD "Build using Qt5 libs" TRUE)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(lib/StanfordCPPLib)
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
link_libraries(stanford)
add_executable(app ${app_SOURCES})
IF (QT5BUILD)
    qt5_use_modules(app Core)
ENDIF()
file(COPY lib/spl.jar
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})