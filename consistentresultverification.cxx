#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"

using namespace std;
using namespace nlohmann;

int main(int argc, char** argv) {
  ifstream file1;
  ifstream file2;
  json jsonOutput;
  json jsonInput1;
  json jsonInput2;
  int arraySize1 = 0;
  int arraySize2 = 0;
  int numSamples1 = 0;
  int numSamples2 = 0;
  int sampleWithConflictingResults = 0;
    bool isConflicting = false;

  file1.open(argv[1]);
  if(file1.is_open()){
    file1 >> jsonInput1;
  }
  arraySize1 = jsonInput1["metadata"]["arraySize"];
  numSamples1 = jsonInput1["metadata"]["numSamples"];

  file2.open(argv[2]);
  if(file2.is_open()){
    file2 >> jsonInput2;
  }
  arraySize2 = jsonInput2["metadata"]["arraySize"];
  numSamples2 = jsonInput2["metadata"]["numSamples"];

  auto itr2 = jsonInput2.begin();
  for(auto itr1 = jsonInput1.begin();++itr1 != jsonInput1.end();++itr1){
    --itr1;
    string key = itr1.key();
    int i = 0;
    auto arrayItr2 = jsonInput2[key].begin();
    for (auto arrayItr1 = jsonInput1[key].begin(); arrayItr1 != jsonInput1[key].end();++arrayItr1){
      if(*arrayItr1 != *arrayItr2){
	isConflicting = true;
	jsonOutput[key]["Mismatches"][to_string(i)] = {*arrayItr1,*arrayItr2};
      }
      arrayItr2++;
      i++;
    }

    if(isConflicting){
      sampleWithConflictingResults++;
      jsonOutput[key][argv[1]] = itr1.value();
      jsonOutput[key][argv[2]] = itr2.value();
      isConflicting = false;
    }
    itr2++;
  }

  jsonOutput["metadata"]["File1"]["arraysize"] = arraySize1;
  jsonOutput["metadata"]["File1"]["name"] = argv[1];
  jsonOutput["metadata"]["File1"]["numSamples"] = numSamples1;
  jsonOutput["metadata"]["File2"]["arraysize"] = arraySize2;
  jsonOutput["metadata"]["File2"]["name"] = argv[2];
  jsonOutput["metadata"]["File2"]["numSamples"] = numSamples2;
  jsonOutput["metadata"]["sampleWithConflictingResults"] = sampleWithConflictingResults;

  cout << jsonOutput.dump(2) << "\n";
}
