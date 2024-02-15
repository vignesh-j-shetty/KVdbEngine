#include <gtest/gtest.h>
#include "Page.h"

class PageTestFixture: public testing::Test {
  protected:
  char *buffer = nullptr;
  char *endBufferPointer = nullptr;
  char *temporaryBuffer = nullptr;
  Page *page;
  void SetUp() override {
    buffer = new char[DISKMANAGER_PAGESIZE];
    temporaryBuffer = new char[DISKMANAGER_PAGESIZE];
    endBufferPointer = buffer + DISKMANAGER_PAGESIZE;
    page = new Page(buffer, 2024);
    resetBuffer();
  }

  void TearDown() override {
    delete page;
    resetBuffer();
  }

  void resetBuffer() {
    for(uint16 i = 0; i < DISKMANAGER_PAGESIZE; i++) {
      buffer[i] = 0;
    }
  }
};

TEST_F(PageTestFixture, CheckInsert) {
  resetBuffer();
  std::string insertData = "Hello World";
  page->insertRecord(insertData.data(), insertData.size(), 0);
  std::string pageData(endBufferPointer - insertData.size(), insertData.size());
  EXPECT_TRUE(insertData == pageData);
}

TEST_F(PageTestFixture, CheckRead) {
  resetBuffer();
  std::string insertData = "Hello World";
  page->insertRecord(insertData.data(), insertData.size(), 0);
  page->readRecord(temporaryBuffer, insertData.size(), 0);
  std::string pageData(temporaryBuffer, insertData.size());
  EXPECT_TRUE(insertData == pageData);
}

TEST_F(PageTestFixture, CheckMultiInsert) {
  resetBuffer();
  std::string insertData1 = "HelloWorld1";
  page->insertRecord(insertData1.data(), insertData1.size(), 0);
  std::string insertData2 = "HelloWorld2";
  page->insertRecord(insertData2.data(), insertData2.size(), 0);
  std::string pageData(endBufferPointer - insertData1.size() - insertData2.size() - PAGE_RECORD_HEADER_SIZE, insertData2.size());
  EXPECT_TRUE(pageData == insertData2);
}

TEST_F(PageTestFixture, CheckInsertOrder_a) {
  resetBuffer();
  std::string insertData = "HelloWorld1";
  page->insertRecord(insertData.data(), insertData.size(), 0);
  insertData = "HelloWorld2";
  page->insertRecord(insertData.data(), insertData.size(), 0);
  page->readRecord(temporaryBuffer, page->getRecordSize(1), 1);
  std::string pageData(temporaryBuffer, page->getRecordSize(1));
  EXPECT_TRUE(pageData == "HelloWorld1");
}

TEST_F(PageTestFixture, CheckInsertOrder_b) {
  resetBuffer();
  std::string insertData = "Hello-World1";
  page->insertRecord(insertData.data(), insertData.size(), 0);
  insertData = "HelloWorld2";
  page->insertRecord(insertData.data(), insertData.size(), 0);
  insertData = "HelloWorld333";
  page->insertRecord(insertData.data(), insertData.size(), 2);
  const uint8 readIndex = 2;
  page->readRecord(temporaryBuffer, page->getRecordSize(readIndex), readIndex);
  std::string pageData(temporaryBuffer, page->getRecordSize(readIndex));
  EXPECT_TRUE(pageData == "HelloWorld333");
}

TEST_F(PageTestFixture, CheckInsertOrder_c) {
  resetBuffer();
  std::string insertData = "Hello-World1";
  page->insertRecord(insertData.data(), insertData.size(), 0);
  insertData = "HelloWorld2";
  page->insertRecord(insertData.data(), insertData.size(), 0);
  insertData = "HelloWorld333";
  page->insertRecord(insertData.data(), insertData.size(), 1);
  const uint8 readIndex = 2;
  page->readRecord(temporaryBuffer, page->getRecordSize(readIndex), readIndex);
  std::string pageData(temporaryBuffer, page->getRecordSize(readIndex));
  EXPECT_TRUE(pageData == "Hello-World1");
}

TEST_F(PageTestFixture, Split) {
  resetBuffer();
  std::string insertData = "Hello-World1";
  page->insertRecord(insertData.data(), insertData.size(), 0);
  insertData = "HelloWorld2";
  page->insertRecord(insertData.data(), insertData.size(), 1);
  insertData = "HelloWorld3";
  page->insertRecord(insertData.data(), insertData.size(), 2);
  insertData = "HelloWorld4";
  page->insertRecord(insertData.data(), insertData.size(), 3);
  insertData = "HelloWorld5";
  page->insertRecord(insertData.data(), insertData.size(), 4);
  char *splittedPageBuffer = new char[DISKMANAGER_PAGESIZE];
  for(uint16 i = 0; i < DISKMANAGER_PAGESIZE; i++) {
    splittedPageBuffer[i] = 0;
  }
  std::shared_ptr<Page> splittedPage(new Page(splittedPageBuffer, 2023));
  page->split(splittedPage);
  EXPECT_TRUE(splittedPage->getRecordCount() == 2);
}