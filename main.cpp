#include <iostream>
#include <fstream>

using namespace std;

#include "Utilities.h"

#include "BlockChain.h"


using std::endl;


void format(std::ifstream &source, std::ofstream &target);

void h(std::ifstream &source, std::ofstream &target);

void compress(std::ifstream &source, std::ofstream &target);

void verify(std::ifstream &source, std::ifstream &target);


int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::string f = getErrorMessage();
        cout << f << endl;
        return 1;
    }
    std::string op = argv[1];
    std::string s = argv[2];
    std::string t = argv[3];

    std::ifstream source(s);


    if (op == "format") {
        std::ofstream target(t);
        format(source, target);
        target.close();
    }else if (op == "hash") {
        std::ofstream target(t);
        h(source, target);
        target.close();
    }
   else  if (op == "compress") {
        std::ofstream target(t);
        compress(source, target);
        target.close();
    }
    else if (op == "verify") {
        std::ifstream target(t);
        verify(source, target);
        target.close();
    }
    else{cout << getErrorMessage() << endl;}

    source.close();

}

void format(std::ifstream &source, std::ofstream &target) {
    BlockChain head = BlockChainLoad(source);
    BlockChainDump(head, target);
    clearBlockchain(head);
}

void h(std::ifstream &source, std::ofstream &target) {
    BlockChain head = BlockChainLoad(source);
    BlockChainDumpHashed(head, target);
    clearBlockchain(head);
}

void compress(std::ifstream &source, std::ofstream &target) {
    BlockChain head = BlockChainLoad(source);
    BlockChainCompress(head);
    BlockChainDump(head, target);
    clearBlockchain(head);
}

void verify(std::ifstream &source, std::ifstream &target) {
    BlockChain head = BlockChainLoad(source);
    bool a = BlockChainVerifyFile(head, target);
    if (a) {
        cout << "Verification passed" << endl;
    } else {
        cout << "Verification failed" << endl;
    }
    clearBlockchain(head);
}
