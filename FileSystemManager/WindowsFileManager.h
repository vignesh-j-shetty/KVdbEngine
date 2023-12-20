#ifndef WINDOWSFILEMANAGER
#define WINDOWSFILEMANAGER
#include <string>
#include<iostream>
#include "TypeDefs.h"
#include "FileManager.h"
#include<windows.h>

class WindowsFileManager: public FileManager {
    public:
    WindowsFileManager(std::string fileName);
    virtual ~WindowsFileManager() override;
    virtual bool open() override;
    virtual bool exists() override;
    virtual void seek(uint64 pos, SeekType type = SET) override;
    virtual uint32 write(void *buffer, int count) override;
    virtual uint32 read(void *buffer, int count) override;
    virtual uint64 currentPosition() override;
    virtual void close() override;
    private:
    HANDLE hFile;
    char *cfileName;
    DWORD position;
};
#endif