#include <iostream>
#include <fstream>
#include <string>

#include "Transaction.h"
#include "Utilities.h"


using std::endl;

void TransactionDumpInfo(const Transaction &transaction, ofstream &file) {
    file << "Sender Name:" << transaction.sender << endl;
    file << "Receiver Name:" << transaction.receiver << endl;
    file << "Transaction Value:" << transaction.value << endl;
}

string TransactionHashedMessage(const Transaction &transaction) {
    std::string f = hash((int) transaction.value, transaction.sender, transaction.receiver);
    return f;
}

bool TransactionVerifyHashedMessage(const Transaction &transaction, string hashedMessage) {
    string f = TransactionHashedMessage(transaction);
    if (f == hashedMessage)return true;
    return false;
}
