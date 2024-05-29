cmake_minimum_required(VERSION 3.18)
project(DatabaseAPI)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_CXX_STANDARD 20)
include(GNUInstallDirs)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/DownloadDependecies.cmake)

option(BUILD_SHARED_LIBS ON)

set(SOURCE_FILES
    include/Database/DatabaseAPI.hpp
    src/DatabaseAPI.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(BUILD_SHARED_LIBS)
    add_library(DatabaseAPI SHARED ${SOURCE_FILES})
else()
    add_library(DatabaseAPI STATIC ${SOURCE_FILES})
endif()

set(LIBRARIES SQLite)

target_link_libraries(DatabaseAPI ${LIBRARIES})

# Build examples
add_executable(UpdateValues_example ${CMAKE_CURRENT_SOURCE_DIR}/example/UpdateValues.cpp)
target_link_libraries(UpdateValues_example DatabaseAPI)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/example/DB DESTINATION ${CMAKE_BINARY_DIR}/)