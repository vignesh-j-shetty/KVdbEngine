#include "DiskManager.h"
#include "FileManager.h"
#include "UnixFileManager.h"
#include "WindowsFileManager.h"
#include "DiskManager.h"
#include<cassert>

DiskManager::DiskManager(const char *fileName) {
  #ifdef WIN32
  fileManager = new WindowsFileManager(fileName);
  #else
  fileManager = new UnixFileManager(fileName);
  #endif
  temporaryBuffer = new char[DISKMANAGER_HEADERSIZE];
  if(fileManager->exists() == false) {
    //Create header
    assert(fileManager->open());
    headFreePageID = headAllocatedPageID = DISK_NULL;
    updateHeader();
    std::cout<<"New db file successfully created"<<std::endl;
  } else {
    //Setup Header values
    fileManager->open();
    char *buffer = temporaryBuffer;
    fileManager->read(buffer, DISKMANAGER_HEADERSIZE);
    memcpy(&headAllocatedPageID, buffer, 8);
    memcpy(&headFreePageID, buffer + 8, 8);
    delete[] buffer;
  }
}

DiskManager::~DiskManager() throw() {
  delete fileManager;
}

void DiskManager::updateHeader() {
  /*
  Header Structure
  Allocated Page Pointer: 8
  Free Page Pointer: 8
  */
  //Header Setup
  uint64 pos = fileManager->currentPosition();
  fileManager->seek(0, SET);
  char *buffer = temporaryBuffer;
  memcpy(buffer, &headAllocatedPageID, 8);
  memcpy(buffer + 8, &headFreePageID, 8);
  assert(fileManager->write(buffer, DISKMANAGER_HEADERSIZE) == DISKMANAGER_HEADERSIZE);
  delete[] buffer;
  //Set back to location where it was before
  fileManager->seek(pos, SET);
}

uint64 DiskManager::getHeadPageID() {
  return headAllocatedPageID;
}

uint64 DiskManager::getHeadFreePageID() {
  return headFreePageID;
}

void DiskManager::writePage(Page &page) {
  fileManager->seek(page.id, SET);
  fileManager->write(page.buffer.get(), DISKMANAGER_PAGESIZE);
}

Page DiskManager::readPage(uint64 id) {
  //Check if id is valid
  assert((id-16)%2024 == 0);
  fileManager->seek(id, SET);
  char *buffer = new char[DISKMANAGER_PAGESIZE];
  assert(fileManager->read(buffer, DISKMANAGER_PAGESIZE) == DISKMANAGER_PAGESIZE);
  Page page(buffer, id);
  return page;
}