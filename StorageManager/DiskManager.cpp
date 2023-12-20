#include "DiskManager.h"
#include<cassert>

DiskManager::DiskManager(const char *fileName) {
  #ifdef WIN32
  fileManager = new WindowsFileManager(fileName);
  #else
  fileManager = new UnixFileManager(fileName);
  #endif
    pageSize = DISKMANAGER_PAGESIZE;
    if(fileManager->exists() == false) {
      //Create header
      if(!fileManager->open()) {
        assert(false);
      }
      headFreePageID = headOwnedPageID = 0;
      updateHeader();
      std::cout<<"New db file successfully created"<<std::endl;
    } else {
      //Setup Header values
      fileManager->open();
      char *buffer = new char[pageSize];
      fileManager->read(buffer, pageSize);
      memcpy(&pageSize, &buffer[0], 4);
      memcpy(&headOwnedPageID, &buffer[4], 8);
      memcpy(&headFreePageID, &buffer[12], 8);
      delete[] buffer;
    }
}

DiskManager::~DiskManager() throw() {
  delete fileManager;
}

void DiskManager::updateHeader() {
    /*
    Header Structure 
    Offset Content                          Size
      0       Page size                        4
      4       Head Occupied Page Pointer       8    
      12      Head Free Page Pointer           8  
    */
   //Header Setup
   uint64 pos = fileManager->currentPosition();
   fileManager->seek(0, SET);
   char *buffer = new char[DISKMANAGER_HEADERSIZE];
   memcpy(buffer, &pageSize, 4);
   memcpy(&buffer[4], &headOwnedPageID, 8);
   memcpy(&buffer[12], &headFreePageID, 8);
   assert(fileManager->write(buffer, DISKMANAGER_HEADERSIZE) == DISKMANAGER_HEADERSIZE);
   delete[] buffer;
   //Set back to location where it was before
   fileManager->seek(pos, SET);
}

uint32 DiskManager::getPageSize() {
  return pageSize;
}

uint64 DiskManager::getHeadPageID() {
  return headOwnedPageID;
}

uint64 DiskManager::getHeadFreePageID() {
  return headFreePageID;
}

void DiskManager::readPage(uint64 pageID, char *buffer) {
  fileManager->seek(pageID, SET);
  fileManager->read(buffer, pageSize);
}

void DiskManager::writePage(uint64 pageID, char *buffer) {
  fileManager->seek(pageID, SET);
  assert(fileManager->write(buffer, pageSize) == pageSize);
}

void DiskManager::createFreePage() {
  char *buffer = new char[pageSize];
  for(int i = 0; i < pageSize; i++) {
    buffer[i] = (char) 0;
  }
  fileManager->seek(0, END);
  uint64 newPageID = fileManager->currentPosition();
  if(headFreePageID != 0) {
    memcpy(buffer, &headFreePageID, sizeof(headFreePageID));;
  }
  headFreePageID = newPageID;
  updateHeader();
  writePage(newPageID, buffer);
  delete[] buffer;
}

void DiskManager::createOwnedPage() {
  char *buffer = new char[pageSize];
  if(headOwnedPageID == 0) {
    if(headFreePageID == 0) {
      createFreePage();
    }
    assert(headFreePageID != 0);
    readPage(headFreePageID, buffer);
    headOwnedPageID = headFreePageID;
    memcpy(&headFreePageID, buffer, sizeof(headFreePageID));
  }
  updateHeader();
}