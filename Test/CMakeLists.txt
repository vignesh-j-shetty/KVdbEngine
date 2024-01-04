include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP false
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(PageTest PageTest.cpp)
target_link_libraries(PageTest GTest::gtest_main Page)
target_include_directories(PageTest PRIVATE ${CMAKE_SOURCE_DIR}/StorageManager)

include(GoogleTest)
gtest_discover_tests(PageTest)
