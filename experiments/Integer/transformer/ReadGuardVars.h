#include <cstdio>
#include <memory>
#include <sstream>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class ReadGuardVars{
  std::vector<std::string> guard_variables;
  public:
  void populateGuardInfo(std::string fileName);
  void printInfo();
  std::vector<std::string> getGuards();
};