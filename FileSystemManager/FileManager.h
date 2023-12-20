#ifndef FILEMANAGER
#define FILEMANAGER
#include "TypeDefs.h"
/*
SET - The file offset is set to offset bytes.
CUR - The file offset is set to its current location plus offset bytes.
END - The file offset is set to the size of the file plus offset bytes.
*/
enum SeekType {SET, CUR, END};

class FileManager {
    public:
    virtual bool open() = 0;

    virtual bool exists() = 0;

    virtual void seek(uint64 pos, SeekType type) = 0;

    virtual uint32 write(void *buffer, int count) = 0;

    virtual uint32 read(void *buffer, int count) = 0;

    virtual uint64 currentPosition() = 0;

    virtual void close() = 0;

    virtual ~FileManager() {};
};

#endif