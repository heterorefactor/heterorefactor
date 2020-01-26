#include <cstdio>
#include <memory>
#include <sstream>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <string>

class ReadInvariants{
  std::map<std::string, int> invariantMap;
  std::map<std::string, std::string> invariantTypeMap;
  public:
  void populateInvariantInfo(std::string fileName);
  void printMap();
  std::map<std::string, int> getInvariants();
  std::map<std::string, std::string> getInvariantTypeMap();
};