add_library(BTNode BTNode.cpp)
add_library(StringKey StringKey.cpp)
add_library(StringValue StringValue.cpp)
add_library(KeyValueFactory KeyValueFactory.cpp)

target_include_directories(BTNode PRIVATE ${CMAKE_SOURCE_DIR}/StorageManager)

target_link_libraries(KeyValueFactory PUBLIC StringKey)
target_link_libraries(KeyValueFactory PUBLIC StringValue)
target_link_libraries(BTNode PRIVATE KeyValueFactory)
target_link_libraries(BTNode PUBLIC Page)