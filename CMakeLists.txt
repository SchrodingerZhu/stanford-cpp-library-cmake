cmake_minimum_required (VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -fPIC")
project(app)
option(QT5BUILD "Build using Qt5 libs" TRUE)
# set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{QT5_DIR} "E:\\Qt\\Qt5.11.3\\5.11.3\\mingw53_32\\")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET( QT_WRAP_CPP TRUE)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(Qt5Multimedia CONFIG REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS} ${Qt5Multimedia_INCLUDE_DIRS})
include_directories(
        lib/StanfordCPPLib
        lib/StanfordCPPLib/io
        lib/StanfordCPPLib/collections
        lib/StanfordCPPLib/system
        lib/StanfordCPPLib/private
        lib/StanfordCPPLib/util
        lib/StanfordCPPLib/graphics)
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

set(app_SOURCES src/hello.cpp)
file(GLOB g_files
        "lib/StanfordCPPLib/graphics/*.cpp"
        )
qt5_wrap_cpp(moc_sources ${g_files})
add_library(stanford ${stanford_files})
target_link_libraries(stanford Qt5::Widgets Qt5::Network Qt5::Core Qt5::Multimedia)
add_executable(app ${OS_BUNDLE} ${app_SOURCES} ${QRCS})
#qt5_use_modules(app Widgets Network Core Multimedia)
target_link_libraries(app stanford Qt5::Widgets Qt5::Network Qt5::Core Qt5::Multimedia)

