cmake_minimum_required (VERSION 3.9.0)

project(TestSimpleList)

set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)

add_executable(${PROJECT_NAME} "") 

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

target_sources(${PROJECT_NAME}
    PUBLIC 
        src/main.cpp 
        src/UTSimpleList.cpp)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_package(cabbages-testd 1.0.0 NAMES CONFIG REQUIRED)
else()
    find_package(cabbages-test 1.0.0 NAMES CONFIG REQUIRED)
endif()


target_link_libraries(${PROJECT_NAME} ${target_name} debug cabbages-testd)
target_link_libraries(${PROJECT_NAME} ${target_name} optimized cabbages-test)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})