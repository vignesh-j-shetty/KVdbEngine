#include "BTree.h"
#include <string>
#include<memory>
#include<iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <set>
#include<vector>

// void print(BTNode &node) {
//     for(uint8 i = 0; i < node.getItemCount(); i++) {
//         std::string key = std::any_cast<std::string>(node.getKey(i)->getData());
//         std::string value = std::any_cast<std::string>(node.getValue(i)->getValue());
//         std::cout<<"Key :"<<key<<" -- ";
//         std::cout<<"Value :"<<value<<std::endl;
//     }
// }

// auto start = std::chrono::high_resolution_clock::now();
// auto end = std::chrono::high_resolution_clock::now();
// std::chrono::duration<double, std::milli> ms_double = end - start;
// std::cout << "Execution time: " << ms_double.count() << " milliseconds\n";


void setup(BTree &btree) {
    std::shared_ptr<Key> key(new UIntKey(0));
    std::shared_ptr<Value> value(new StringValue("fsdfs"));
    uint64 n = 1000000;
    std::cout<<"Size of input : "<<n<<std::endl;
    for (uint64 i = 0; i < n; i++) {
        key->setData(std::any_cast<uint64>(i));
        btree.insert(key, value);
    }
    std::cout<<"\n Done inserting\n";
}

uint64 getGenerateNewNumber(std::set<uint64> &generatedValue, const uint64 maxValue) {
    uint64 value;
    while (true) {
        value = rand() % maxValue;
        auto it = generatedValue.find(value);
        if(it == generatedValue.end()) {
            generatedValue.insert(value);
            break;
        }
    }
    // std::cout<<value<<std::endl;
    return value;
}

void random_insert(BTree &btree, std::vector<uint64> &generatedNumber) {
    const uint64 maxNumber = 100;
    std::set<uint64> generatedValue;
    std::shared_ptr<Key> key(new UIntKey(0));
    std::shared_ptr<Value> value(new StringValue("vignesh"));
    const uint64 itemCount = 10;
    for(uint64 i = 0; i < itemCount; i++) {
        uint64 randomnumber = getGenerateNewNumber(generatedValue, maxNumber);
        generatedNumber.push_back(randomnumber);
        key->setData(std::any_cast<uint64>(randomnumber));
        btree.insert(key, value);
    }
    std::cout<<"\n Done Random inserting\n";
}


int main() {
    std::vector<uint64> generatedNumbers;
    {
        BTree btree;
        random_insert(btree, generatedNumbers);
    }
    BTree btree;
    // for(uint64 i = 0; i < generatedNumbers.size(); i++) {
    //     std::shared_ptr<Key> key(new UIntKey(generatedNumbers[i]));
    //     if(!btree.isKeyPresent(key)) {
    //         std::cout<<"Error "<<generatedNumbers[i]<<" "<<i<<std::endl;
    //     }
    // }
    std::shared_ptr<Key> key(new UIntKey(3));
    btree.debugPrint();
    const uint16 n1 = 3;
    uint64 keysToRemove1[n1] = {23, 30, 44};
    for(uint16 i = 0; i < n1; i++) {
        key->setData(keysToRemove1[i]);
        btree.remove(key);
    }
    std::cout<<"\nAfter deletion 1\n";
    btree.debugPrint();
    const uint16 n2 = 2;
    uint64 keysToRemove2[n2] = {9, 7};
    for(uint16 i = 0; i < n2; i++) {
        key->setData(keysToRemove2[i]);
        btree.remove(key);
    }
    std::cout<<"\nAfter deletion 2\n";
    btree.debugPrint();
    std::cout<<"\n---END---- ";
    return 0;
}