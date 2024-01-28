#include "UnixFileManager.h"
#include <fcntl.h>
#include <unistd.h>

uint32 open_rdwr(char *cfileName) {
    return open(cfileName, O_RDWR | O_CREAT,S_IWUSR | S_IRUSR);
}

uint32 open_rd(char *cfileName) {
    return open(cfileName, O_RDONLY);
}

void close_(int fd) {
    close(fd);
}

uint32 write_(int fd, void *buffer, int count) {
    return write(fd,buffer,count);
}

uint32 read_(int fd, void *buffer, int count) {
    return read(fd,buffer,count);
}


UnixFileManager::UnixFileManager(std::string fileName) {
    int len = fileName.length();
    cfileName = new char[len];
    strcpy(cfileName,fileName.c_str());
    fd = -1;
}

UnixFileManager::~UnixFileManager() {
    delete cfileName;
    close();
}

bool UnixFileManager::open() {
    fd = open_rdwr(cfileName);
    if(fd == -1) {
        return false;
    }
    return true;
}

bool UnixFileManager::exists() {
    int fd_ = open_rd(cfileName);
    close_(fd_);
    if(fd_ < 0) {
        return false;
    }
    return true;
}

uint64 UnixFileManager::seek(uint64 pos, SeekType type) {
    uint64 position;
    switch (type) {
        case SET:
        position = lseek(fd,pos,SEEK_SET);
        break;
        case CUR:
        position = lseek(fd,pos,SEEK_CUR);
        break;
        case END:
        position = lseek(fd,pos,SEEK_END);
        break;
        default:
        break;
    }
    return position;
}

uint32 UnixFileManager::write(void *buffer, int count) {
    return write_(fd,buffer,count);
}

uint32 UnixFileManager::read(void *buffer, int count) {
    return read_(fd,buffer,count);
}

uint64 UnixFileManager::getCurrentPosition() {
    return lseek(fd, 0, SEEK_CUR);
}

void UnixFileManager::close() {
    close_(fd);
}