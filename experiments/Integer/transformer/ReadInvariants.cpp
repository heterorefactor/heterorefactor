#include "ReadInvariants.h"

void ReadInvariants::populateInvariantInfo(std::string fileName){
    std::string line;
    std::ifstream myfile (fileName);
    if (myfile.is_open())
    {
      int i=1;
      std::string name;
     while (getline (myfile,line) )
      {        
        if(i==1){
          name = line;
        } else{
          if(i == 2){
            invariantMap[name] = atoi(line.c_str());
          } else{
             invariantTypeMap[name] = line; 
             i=0;
          }
        }
        i = i+1;
      }
     myfile.close();
    }
    else printf("%s\n","Unable to open file"); 
}

void ReadInvariants::printMap(){
  for (auto it = invariantMap.cbegin(); it != invariantMap.cend(); ++it) {
          std::cout << "{" << (*it).first << ": " << (*it).second << "}\n";
    }
}

std::map<std::string, int> ReadInvariants::getInvariants(){
      return invariantMap;
}

std::map<std::string, std::string> ReadInvariants::getInvariantTypeMap(){
  return invariantTypeMap;
}