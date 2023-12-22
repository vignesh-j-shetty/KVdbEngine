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
  temporaryBuffer = new char[DISKMANAGER_PAGESIZE];
  if(fileManager->exists() == false) {
    //Create header
    assert(fileManager->open());
    headFreePageID = rootPageID = DISK_NULL;
    updateHeader();
    std::cout<<"New db file successfully created"<<std::endl;
  } else {
    assert(fileManager->open());
    //Read from header
    readHeader();
    createRootPage();
  }
}

DiskManager::~DiskManager() throw() {
  delete fileManager;
  delete[] temporaryBuffer;
}

void DiskManager::updateHeader() {
  /*
  Header Structure
  Root Page Pointer: 4
  Free Page Pointer: 4
  */
  //Header Setup
  memcpy(temporaryBuffer, &rootPageID, 4);
  memcpy(temporaryBuffer + 4, &headFreePageID, 4);
  fileManager->seek(0, SET);
  assert(fileManager->write(temporaryBuffer, DISKMANAGER_HEADERSIZE) == DISKMANAGER_HEADERSIZE);
}

void DiskManager::readHeader() {
  fileManager->seek(0, SET);
  assert(fileManager->read(temporaryBuffer, DISKMANAGER_HEADERSIZE) == DISKMANAGER_HEADERSIZE);
  memcpy(&rootPageID, temporaryBuffer, 4);
  memcpy(&headFreePageID, temporaryBuffer + 4, 4);
}

void DiskManager::writePage(Page &page) {
  fileManager->seek(page.id, SET);
  assert(fileManager->write(page.buffer.get(), DISKMANAGER_PAGESIZE) == DISKMANAGER_PAGESIZE);
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

void DiskManager::createRootPage() {
  fileManager->seek(0, END);
  for(int i = 0; i < DISKMANAGER_PAGESIZE; i++) {
    temporaryBuffer[i] = PAGE_FILL;
  }
  fileManager->write(temporaryBuffer, DISKMANAGER_PAGESIZE);
}