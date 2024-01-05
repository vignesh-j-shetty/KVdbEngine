#include <gtest/gtest.h>
#include "BTNode.h"

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
};

TEST_F(BTNodeTest, CheckInsert) {
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