#ifndef BTNODE
#define BTNODE
#define NODE_SIZE 100
#include "TypeDefs.h"
#include "BTKeyValue.h"
#include <Page.h>
enum NodeType {
    ROOT_NODE = 0, INTERNAL_NODE = 1, BT_LEAF_NODE = 2
};

class BTNode {
    public:
    BTNode(Page page);
    void insertKeyValue(BTKeyValuePair keyValuePair);
    private:
    Page page;
    LinearIterator<uint16> *pageIterator;
    //Returns true if key is 
    bool keyCompareSm(BTKeyValuePair keyValuePair1, BTKeyValuePair keyValuePair2);
    bool cmp(char *str1, char *str2);
};
#endif