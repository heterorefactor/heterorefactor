#include "BitSavings.h"

void BitSavings::populateVars(std::string varName){
    modifiedVars.push_back(varName);
}
void BitSavings::addToOriginalMap(std::string varName,int size){
    originalBitSize[varName] = size;
}

void BitSavings::addToModifiedMap(std::string varName,int size){
    modifiedBitSize[varName] = size;
}
void BitSavings::printInfo(){
    int origSize=0;
    int optSize=0;    
    for (auto it = originalBitSize.cbegin(); it != originalBitSize.cend(); ++it) {
          std::cout << "{" << (*it).first << ": " << (*it).second << "}\n";
          origSize+=(*it).second;
    }

    for (auto it = modifiedBitSize.cbegin(); it != modifiedBitSize.cend(); ++it) {
          std::cout << "{" << (*it).first << ": " << (*it).second << "}\n";
          optSize+=(*it).second;
    }

    std::cout<<"Original Total Bit Width: "<<origSize<<"\n";
    std::cout<<"Optimized Total Bit Width: "<<optSize<<"\n";
}