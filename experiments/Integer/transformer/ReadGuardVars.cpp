#include "ReadGuardVars.h"

void ReadGuardVars::populateGuardInfo(std::string fileName){
    std::string line;
    // std::ifstream myfile ("/Users/Aish/Documents/llvm-project/clang/tools/transformer/invariants.txt");
    std::ifstream myfile (fileName);
    if (myfile.is_open())
    {
      int i=1;
      std::string name;
     while (getline (myfile,line) )
      {    
        guard_variables.push_back(line);            
      }
     myfile.close();
    }
    else printf("%s\n","Unable to open guard variable file"); 
}

void ReadGuardVars::printInfo(){
  for (auto i = guard_variables.begin(); i != guard_variables.end(); ++i) 
        std::cout << *i << " "; 
}

std::vector<std::string>  ReadGuardVars::getGuards(){
      return guard_variables;
}