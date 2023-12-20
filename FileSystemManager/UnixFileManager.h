#ifndef UNIXFILEMANAGER
#define UNIXFILEMANAGER
#include <string>
#include<iostream>
#include "TypeDefs.h"
#include "FileManager.h"

class UnixFileManager: public FileManager {
    public:
    UnixFileManager(std::string fileName);
    virtual ~UnixFileManager() override;
    virtual bool open() override;
    virtual bool exists() override;
    virtual void seek(uint64 pos, SeekType type = SET) override;
    virtual uint32 write(void *buffer, int count) override;
    virtual uint32 read(void *buffer, int count) override;
    virtual uint64 currentPosition() override;
    virtual void close() override;
    private:
    int fd;
    char *cfileName;
    uint64 position;
};
#endif