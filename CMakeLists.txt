cmake_minimum_required(VERSION 3.23)
project(ImageFusion)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(gflags REQUIRED)


set(LIB
        gflags
        ${OpenCV_LIBRARIES})

set(DEP_XML_INCLUDE_DIR
        dependencies/pugixml/src)
set(DEP_XML_SRC_DIR
        dependencies/pugixmlsrc)

set(DEP_INCLUDE_DIR
        ${DEP_XML_INCLUDE_DIR})

set(DEP_SRC
        ${DEP_XML_INCLUDE_DIR}/pugixml.cpp)

include_directories(
        ${DEP_INCLUDE_DIR})



add_executable(ImageFusion
        src/main.cpp
        ${DEP_SRC})
target_link_libraries(ImageFusion ${LIB})
