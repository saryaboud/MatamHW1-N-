#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "BlockChain.h"
#include "Utilities.h"


void stream(const std::string &str, std::string &sender, std::string &receiver, int &value, std::string &timestamp);


int BlockChainGetSize(const BlockChain &blockChain) {
    int length = 0; //counter for the how many transaction(blocks)
    const BlockChain *tmp = blockChain.next; //pointer to the head of the blockchain

    while (tmp != nullptr) //moves over the blockchain until it reaches Null(the end)
    {
        length++;
        tmp = tmp->next; //moves the pointer to the next block
    }
    return length ; //return the length of the the blockchain
}

int BlockChainPersonalBalance(const BlockChain &blockChain, const string &name) {
    unsigned int totalCoins = 0;
    const BlockChain *tmp = &blockChain; //pointer to the head of the blockchain
    while (tmp != nullptr) //moves over the blockchain until it reaches Null(the end)
    {
        //if the person is a receiver will add to his coins number
        if (tmp->data.receiver == name)
        {
            totalCoins += tmp->data.value;
        }
        //if the person is a sender will minus to his coins number
        if (tmp->data.sender == name)
        {
            totalCoins -= tmp->data.value;
        }
        tmp = tmp->next; //moves the pointer to the next block
    }
    return (int) totalCoins; // return the final sum (received - sended)
}

void BlockChainAppendTransaction(
    BlockChain &blockChain,
    unsigned int value,
    const string &sender,
    const string &receiver,
    const string &timestamp
) {
    Transaction data;
    data.sender = sender;
    data.receiver = receiver;
    data.value = value;


    auto *newBlock = new BlockChain();
    newBlock->data = data;
    newBlock->timestamp = timestamp;

    newBlock->next = blockChain.next;
    blockChain.next = newBlock;
}

void BlockChainAppendTransaction(
    BlockChain &blockChain,
    const Transaction &transaction,
    const string &timestamp
) { BlockChainAppendTransaction(blockChain,transaction.value
    ,transaction.sender,transaction.receiver,timestamp);
}

BlockChain BlockChainLoad(ifstream &file) {
    BlockChain head;  // dummy head
    head.next = nullptr;
    BlockChain* tmp = &head;

    std::string sender, receiver, timestamp;
    int value;
    while (file >> sender >> receiver >> value >> timestamp) {
        BlockChain *newBlock = new BlockChain();
        newBlock->data.receiver = receiver;
        newBlock->data.sender = sender;
        newBlock->data.value = value;
        newBlock->timestamp = timestamp;
        newBlock->next = nullptr;

        tmp->next = newBlock;
        tmp = newBlock;
    }

    return head; // caller must use head.next
}

void BlockChainDump(const BlockChain &blockChain, ofstream &file) {
    BlockChain *ptr = blockChain.next;
    file << "BlockChain Info:" << endl;
    int counter = 1;
    while(ptr != nullptr){
        file << counter << "." << endl;
        file << "Sender Name: " << ptr->data.sender << endl;
        file << "Receiver Name: " << ptr->data.receiver << endl;
        file << "Transaction Value: " << ptr->data.value << endl;
        file << "Transaction timestamp: " << ptr->timestamp << endl;
        ptr = ptr->next;
        counter++;
    }
}

void BlockChainDumpHashed(const BlockChain &blockChain, std::ofstream &file) {
    const BlockChain *tmp = blockChain.next;
    bool first = true;
    while (tmp != nullptr) {
        if (!first) {
            file << std::endl;
        } else {
            first = false;
        }

        file << TransactionHashedMessage(tmp->data);
        tmp = tmp->next;
    }
}

bool BlockChainVerifyFile(const BlockChain &blockChain, std::ifstream &file) {
    const BlockChain *tmp = blockChain.next;
    std::string str;
    int lineCount = 0;
    while (std::getline(file, str)) {
        if(!TransactionVerifyHashedMessage(tmp->data,str)){
            return false;
        }
        lineCount++;
        tmp = tmp->next;
    }
    if(lineCount != BlockChainGetSize(blockChain)){
        return false;
    }
    return true;
}

bool CheckIfSenderAndRecieverEqual(const BlockChain &block1, const BlockChain &block2) {
    if (block1.data.receiver == block2.data.receiver) {
        if (block1.data.sender == block2.data.sender) {
            return true;
        }
    }
    return false;
}

void BlockChainCompress(BlockChain &blockChain) {
    BlockChain *tmp1 = blockChain.next, *tmp2 = tmp1->next;
    unsigned int sumValue = tmp1->data.value;
    string lastTime = tmp1->timestamp;

    while (tmp2 != nullptr) {
        if (CheckIfSenderAndRecieverEqual(*tmp1, *tmp2)) {
            sumValue += tmp2->data.value;

            BlockChain* tmp3 = tmp2;
            tmp2 = tmp2->next;  // Move tmp2 to the next block
            delete tmp3;        // Delete the current block (tmp2 before move)
        } else {
            // If blocks were compressed, update tmp1 with the new sum and timestamp
            if (sumValue != tmp1->data.value) {
                tmp1->data.value = sumValue;
                tmp1->timestamp = lastTime;
            }

            // Move tmp1 and tmp2 forward
            tmp1->next = tmp2;
            tmp1 = tmp1->next;
            tmp2 = tmp1->next;

            sumValue = tmp1->data.value;
            lastTime =tmp1->timestamp;
        }
    }

    // Final check for the last block in the chain
    if (sumValue != tmp1->data.value) {
        tmp1->data.value = sumValue;
        tmp1->timestamp = lastTime;
        tmp1->next = tmp2;
    }
    tmp1 = nullptr;
    tmp2 = nullptr;
}



void BlockChainTransform(BlockChain &blockChain, const updateFunction function) {
    BlockChain *tmp = &blockChain; //pointer to the head of the blockchain
    while (tmp != nullptr) {
        tmp->data.value = function(tmp->data.value);
        tmp = tmp->next;
    }
}

void stream(const std::string &str, std::string &sender, std::string &receiver, int &value, std::string &timestamp) {
    int k = 0;
    std::string word;
    for (char ch: str) {
        if (ch == ' ') {
            if (k == 0) sender = word;
            else if (k == 1) receiver = word;
            else if (k == 2) value = std::stoi(word);
            k++;
            word = "";
        } else { word += ch; }
    }
    if (k == 3) {
        timestamp = word;
    }
}



void clearBlockchain(BlockChain &head) {
    while (head.next != nullptr) {
        BlockChain *tmp = head.next;
        head.next = tmp->next;
        delete tmp;
    }
    head.data = {};
    head.timestamp.clear();
}
