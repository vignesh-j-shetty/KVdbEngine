cmake_minimum_required(VERSION 3.12)

project(KVdbEngine)
add_executable(main main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/CommonHeaders)
add_subdirectory(${CMAKE_SOURCE_DIR}/FileSystemManager)
add_subdirectory(${CMAKE_SOURCE_DIR}/StorageManager)

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/StorageManager)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/FileSystemManager)
target_link_libraries(main DiskManager)