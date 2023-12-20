include_directories(${CMAKE_SOURCE_DIR}/CommonHeaders)

if(WIN32)
    add_library(WindowsFileManager WindowsFileManager.cpp)
elseif(UNIX)
    add_library(UnixFileManager UnixFileManager.cpp)
else()
    message(FATAL_ERROR "Unsupported operating system")
endif()