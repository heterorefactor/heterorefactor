#include <cstdio>
#include <memory>
#include <sstream>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class BitSavings{
  std::vector<std::string> modifiedVars;
  std::map<std::string, int> originalBitSize;
  std::map<std::string, int> modifiedBitSize;
  public:
  void populateVars(std::string varName);
  void addToOriginalMap(std::string varName,int size);
  void addToModifiedMap(std::string varName,int size);
  void printInfo();  
};