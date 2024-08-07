cmake_minimum_required(VERSION 3.26.4)

project(mcm_discrete)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Optimization flag
set(CMAKE_CXX_FLAGS "-O3")

# Tests
option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

add_subdirectory(src/Model)
add_subdirectory(src/Search_Algorithms)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC Model Search_Algorithms)
target_include_directories(${PROJECT_NAME} PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/src/Model"
                          "${PROJECT_SOURCE_DIR}/src/Search_Algorithms"
                          )
