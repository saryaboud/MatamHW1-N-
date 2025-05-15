#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "BlockChain.h"
#include "Utilities.h"


void stream(const std::string &str, std::string &sender, std::string &receiver, int &value, std::string &timestamp);


int BlockChainGetSize(const BlockChain &blockChain) {
    int length = 0; //counter for the how many transaction(blocks)
    const BlockChain *tmp = &blockChain; //pointer to the head of the blockchain

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
) {
    auto *newHead = new BlockChain();

    newHead->data = transaction;
    newHead->timestamp = timestamp;

    newHead->next = blockChain.next;
    blockChain.next = newHead;
}

/**BlockChain::BlockChain(const std::string &sender, const std::string &receiver, double sum) : sender(sender),
    receiver(receiver), sum(sum), next(nullptr) {
}

BlockChain *BlockChain::creatblock(BlockChain *&head, const std::string &sender, const std::string &receiver,
                                   double sum) {
    BlockChain *newBlock = new BlockChain(sender, receiver, sum);
    newBlock->next = head;
    head = newBlock;
    return head;
}
**/

BlockChain BlockChainLoad(ifstream &file) {
    std::string line;

    BlockChain head;
    head.next = nullptr;

    while (getline(file, line)) {
        std::string sender, receiver, timestamp;
        int value;
        stream(line, sender, receiver, value, timestamp);
        BlockChainAppendTransaction(head, value, sender, receiver, timestamp);
    }
    return head;
}

void BlockChainDump(const BlockChain &blockChain, ofstream &file) {
    BlockChain *tmp = blockChain.next;
    int counter = BlockChainGetSize(blockChain);
    counter--;
    file << "BlockChain Info:\n";
    print(tmp, file, counter);
}

void BlockChainDumpHashed(const BlockChain &blockChain, ofstream &file) {
    BlockChain *tmp = blockChain.next;
    print_hash(tmp->next, file);
    std::string f = TransactionHashedMessage(tmp->data);
    file << f ;

}

bool BlockChainVerifyFile(const BlockChain &blockChain, std::ifstream &file) {
    const BlockChain *tmp = &blockChain;
    bool a = true;
    std::string str;
    while (std::getline(file, str)) {
        std::string f = TransactionHashedMessage(tmp->data);
        if (str != f) {
            return false;
        } else {
            tmp = tmp->next;
        }
    }
    return a;
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
    string lastTime = tmp2->timestamp;

    while (tmp2 != nullptr) {
        if (CheckIfSenderAndRecieverEqual(*tmp1, *tmp2)) {
            sumValue += tmp2->data.value;
            lastTime = tmp2->timestamp;

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

void print(BlockChain *tmp, ofstream &file, int counter) {
    if (tmp == nullptr) return;
    print(tmp->next, file, counter - 1);
    file << counter << "." << endl;
    file << "Sender Name: " << tmp->data.sender << endl;
    file << "Receiver Name: " << tmp->data.receiver << endl;
    file << "Transaction Value: " << tmp->data.value << endl;
    file << "Transaction timestamp: " << tmp->timestamp << endl;
}

void print_hash(BlockChain *tmp, ofstream &file) {
    if (tmp == nullptr) return;
    print_hash(tmp->next, file);
    std::string f = TransactionHashedMessage(tmp->data);
    file << f << endl;
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
