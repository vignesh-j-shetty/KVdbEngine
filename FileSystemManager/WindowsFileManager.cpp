#include "WindowsFileManager.h"
#include<cassert>
WindowsFileManager::WindowsFileManager(std::string fileName) {
    int len = fileName.length();
    cfileName = new char[len + 1];
    strcpy_s(cfileName, len + 1, fileName.c_str());
    position = 0;
}

WindowsFileManager::~WindowsFileManager() {
    delete[] cfileName;
    close();
}

bool WindowsFileManager::open() {
    hFile = CreateFileA(cfileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    return hFile != INVALID_HANDLE_VALUE;
}

bool WindowsFileManager::exists() {
    DWORD fileAttributes = GetFileAttributes(cfileName);
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_PATH_NOT_FOUND) {
            return false;
        }
    }
    return true;
}

void WindowsFileManager::seek(uint64 pos, SeekType type) {
    DWORD dwMoveMethod = FILE_BEGIN;
    switch (type) {
        case SET:
            dwMoveMethod = FILE_BEGIN;
            break;
        case CUR:
            dwMoveMethod = FILE_CURRENT;
            break;
        case END:
            dwMoveMethod = FILE_END;
            break;
    }
    position = SetFilePointer(hFile, pos, NULL, dwMoveMethod);
}

uint32 WindowsFileManager::write(void *buffer, int count) {
    DWORD dwBytesWritten;
    if (!WriteFile(hFile, buffer, count, &dwBytesWritten, NULL)) {
        std::cout<<"Write failed :"<<GetLastError()<<std::endl;
        return 0;
    }
    return static_cast<uint32>(dwBytesWritten);
}

uint32 WindowsFileManager::read(void *buffer, int count) {
    DWORD dwBytesRead;
    if(!ReadFile(hFile, buffer, count, &dwBytesRead, NULL)) {
        return 0;
    }
    return static_cast<uint32>(dwBytesRead);
}

uint64 WindowsFileManager::currentPosition() {
    return position;
}

void WindowsFileManager::close() {
     CloseHandle(hFile);
}
