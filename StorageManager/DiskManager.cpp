#include "DiskManager.h"
#include "FileManager.h"
#include "UnixFileManager.h"
#include "WindowsFileManager.h"
#include "DiskManager.h"
#include <cassert>

DiskManager::DiskManager(const char *fileName) {
  #ifdef WIN32
  fileManager = std::shared_ptr<WindowsFileManager>(new WindowsFileManager(fileName));
  #else
  fileManager = std::shared_ptr<UnixFileManager>(new UnixFileManager(fileName));
  #endif
  temporaryBuffer = new char[DISKMANAGER_PAGESIZE];
  if(fileManager->exists() == false) {
    //Create header
    assert(fileManager->open());
    Header header;
    header.freeHeadPageID = header.rootPageID = 0;
    write(header);
    std::cout<<"New db file successfully created"<<std::endl;
  } else {
    assert(fileManager->open());
  }
}

DiskManager::DiskManager(DiskManager &diskManager) {
  temporaryBuffer = new char[DISKMANAGER_PAGESIZE];
  fileManager = diskManager.fileManager;
}

DiskManager::~DiskManager() throw() {
  delete[] temporaryBuffer;
}

void DiskManager::write(Header header) {
  memcpy(temporaryBuffer, &(header.rootPageID), sizeof(header.rootPageID));
  memcpy(temporaryBuffer + sizeof(header.rootPageID), &(header.freeHeadPageID), sizeof(header.freeHeadPageID));
  fileManager->seek(0, SET);
  assert(fileManager->write(temporaryBuffer, DISKMANAGER_HEADERSIZE) == DISKMANAGER_HEADERSIZE);
}

void DiskManager::read(Header &header) {
  fileManager->seek(0, SET);
  assert(fileManager->read(temporaryBuffer, DISKMANAGER_HEADERSIZE) == DISKMANAGER_HEADERSIZE);
  uint64 temp;
  memcpy(&temp, temporaryBuffer, sizeof(header.rootPageID));
  header.rootPageID = temp;
  memcpy(&temp, temporaryBuffer + sizeof(header.freeHeadPageID), sizeof(header.freeHeadPageID));
  header.freeHeadPageID = temp;
}

void DiskManager::writePage(std::shared_ptr<Page> page) {
  fileManager->seek(page->id, SET);
  assert(fileManager->write(page->buffer, DISKMANAGER_PAGESIZE) == DISKMANAGER_PAGESIZE);
}

std::shared_ptr<Page> DiskManager::readPage(uint64 id) {
  //Check if id is valid
  assert((id-16)%2024 == 0);
  fileManager->seek(id, SET);
  char *buffer = new char[DISKMANAGER_PAGESIZE];
  assert(fileManager->read(buffer, DISKMANAGER_PAGESIZE) == DISKMANAGER_PAGESIZE);
  std::shared_ptr<Page> page(new Page(buffer, id));
  return page;
}

std::shared_ptr<Page> DiskManager::createPage() {
  Header header;
  read(header);
  if(header.freeHeadPageID == 0) {
    fileManager->seek(0, END);
    uint64 newPageId = fileManager->getCurrentPosition();
    char *buffer = new char[DISKMANAGER_PAGESIZE];
    fillPageNULL(buffer);
    assert(fileManager->write(buffer, DISKMANAGER_PAGESIZE) == DISKMANAGER_PAGESIZE);
    return std::shared_ptr<Page>(new Page(buffer, newPageId));
  }
  std::shared_ptr<Page> freePage = readPage(header.freeHeadPageID);
  uint64* nextFreePageID = (uint64*) freePage->buffer;
  fillPageNULL(freePage->buffer);
  header.freeHeadPageID = *nextFreePageID;
  write(header);
  return freePage;
}

void DiskManager::deletePage(uint64 id) {
  Header header;
  read(header);
  if(header.freeHeadPageID == 0) {
    header.freeHeadPageID = id;
    write(header);
    setFreePageNextPageID(id, 0);
    return;
  }
  uint64 currentPageID = header.freeHeadPageID;
  uint64 previousPageID = 0;
  bool isInserted = false;
  while (currentPageID != 0) {
    if(currentPageID > id) {
      setFreePageNextPageID(id, currentPageID);
      if(currentPageID == header.freeHeadPageID) {
        header.freeHeadPageID = id;
        write(header);
      } else {
        setFreePageNextPageID(previousPageID, id);
      }
      isInserted = true;
    } else if(currentPageID == id) {
      throw;
    }
    previousPageID = currentPageID;
    currentPageID = getFreePageNextPageID(currentPageID);
  }
  // If not inserted append to end of list
  if(!isInserted) {
    setFreePageNextPageID(previousPageID, id);
    setFreePageNextPageID(id, 0);
  }
}

uint64 DiskManager::createNewPage() {
  fileManager->seek(0, END);
  uint64 position = fileManager->getCurrentPosition();
  fillPageNULL(temporaryBuffer);
  fileManager->write(temporaryBuffer, DISKMANAGER_PAGESIZE);
  return position;
}

inline void DiskManager::fillPageNULL(char *buffer) {
  for(int i = 0; i < DISKMANAGER_PAGESIZE; i++) {
    buffer[i] = PAGE_FILL;
  }
}

inline void DiskManager::setFreePageNextPageID(uint64 id, uint64 nextPageID) {
  uint64* _nextPageID = (uint64*) temporaryBuffer;
  *_nextPageID = nextPageID;
  fileManager->seek(id, SET);
  assert(fileManager->write(temporaryBuffer, FREE_PAGE_HEADER_SIZE) == FREE_PAGE_HEADER_SIZE);
}

inline uint64 DiskManager::getFreePageNextPageID(uint64 id) {
  fileManager->seek(id, SET);
  assert(fileManager->read(temporaryBuffer, FREE_PAGE_HEADER_SIZE) == FREE_PAGE_HEADER_SIZE);
  uint64* _nextPageID = (uint64*) temporaryBuffer;
  return *_nextPageID;
}