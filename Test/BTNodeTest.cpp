#include <gtest/gtest.h>
#include "BTNode.h"
#include <vector>
class BTNodeTest: public testing::Test {
    protected:
    BTNode node;
    char *buffer = nullptr;
    void SetUp() override {
        buffer = new char[DISKMANAGER_PAGESIZE];
        resetBuffer();
        std::shared_ptr<Page> page(new Page(buffer, 0));
        node = BTNode(page);
    }

    void resetBuffer() {
        for(uint16 i = 0; i < DISKMANAGER_PAGESIZE; i++) {
            buffer[i] = 0;
        }
    }

    void setData_a() {
        resetBuffer();
        std::shared_ptr<Key> key(new StringKey("HelloKey1"));
        std::shared_ptr<Value> value(new StringValue("Hello"));
        node.insert(key, value);
        key->setData(std::string("HelloKey2"));
        node.insert(key, value);
        key->setData(std::string("HelloKey3"));
        node.insert(key, value);
        key->setData(std::string("HelloKey4"));
        node.insert(key, value);
    }
};

TEST_F(BTNodeTest, CheckInsert) {
    resetBuffer();
    std::shared_ptr<Key> key(new StringKey("key11"));
    std::shared_ptr<Value> value(new StringValue("Hello"));
    node.insert(key, value);
    std::string keyValue = "Abc";
    key->setData(keyValue);
    node.insert(key, value);
    std::string storedKey = std::any_cast<std::string>(node.getKey(0)->getData());
    EXPECT_TRUE(storedKey == "Abc");
}

TEST_F(BTNodeTest, CheckInsert_a) {
    resetBuffer();
    std::shared_ptr<Key> key(new StringKey("key11"));
    std::shared_ptr<Value> value(new StringValue("Hello"));
    node.insert(key, value);
    std::string keyValue = "Abc";
    key->setData(keyValue);
    node.insert(key, value);
    std::string storedKey = std::any_cast<std::string>(node.getKey(1)->getData());
    EXPECT_TRUE(storedKey == "key11");
}

TEST_F(BTNodeTest, CheckInsert_b) {
    resetBuffer();
    std::shared_ptr<Key> key(new StringKey("azb"));
    std::shared_ptr<Value> value(new StringValue("Hello"));
    node.insert(key, value);
    std::string keyValue = "zzzz";
    key->setData(keyValue);
    node.insert(key, value);
    keyValue = "abc";
    key->setData(keyValue);
    node.insert(key, value);
    std::string storedKey = std::any_cast<std::string>(node.getKey(1)->getData());
    EXPECT_TRUE(storedKey == "azb");
}

TEST_F(BTNodeTest, CheckDelete_a) {
    resetBuffer();
    setData_a();
    node.remove(2);
    std::vector<std::string> expectedElements = {"HelloKey1", "HelloKey2", "HelloKey4"};
    for(uint8 i = 0; i < 3; i++) {
        std::string storedElement = std::any_cast<std::string>(node.getKey(i)->getData());
        EXPECT_TRUE(storedElement == expectedElements[i]);
    }
}

TEST_F(BTNodeTest, CheckDelete_b) {
    resetBuffer();
    setData_a();
    node.remove(3);
    std::vector<std::string> expectedElements = {"HelloKey1", "HelloKey2", "HelloKey3"};
    for(uint8 i = 0; i < 3; i++) {
        std::string storedElement = std::any_cast<std::string>(node.getKey(i)->getData());
        EXPECT_TRUE(storedElement == expectedElements[i]);
    }
}