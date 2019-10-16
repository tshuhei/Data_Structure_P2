#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"

using namespace std;
using namespace nlohmann;
int main(int argc, char** argv) {
  ifstream file;
  json jsonOutput;
  json jsonInput;
  int arraySize = 0;
  int numSamples = 0;
  int sampleWithInversions = 0;
  bool isInversions = false;

  file.open(argv[1]);
  if(file.is_open()){
    file >> jsonInput;
  }
  arraySize = jsonInput["metadata"]["arraySize"];
  numSamples = jsonInput["metadata"]["numSamples"];

  for(auto itr = jsonInput.begin(); ++itr != jsonInput.end(); ++itr){
    --itr;
    string key = itr.key();
    int i =0;
    
    for(auto arrayItr = jsonInput[key].begin(); arrayItr != jsonInput[key].end();++arrayItr){
      auto arraynext = ++arrayItr;
      arrayItr--;
      try{
	if(arraynext  == jsonInput[key].end()){
	  break;
	}
      }catch(json::invalid_iterator& e){
	cout <<"error1" << "\n";
	cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ":" << e.what() << "\n";
	cout << "i = " << i << "\n";
      }
      try{
	if(*arrayItr > *(arraynext)){
	  isInversions = true;
	  jsonOutput[key]["ConsecutiveInversions"][to_string(i)] = {*arrayItr, *arraynext};
	}
      }catch(json::invalid_iterator& e){
	cout <<"error2" << "\n";
	cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ":" << e.what() << "\n";
	cout << "i = " << i << "\n";
      }
      i++;     
    }
    
    if(isInversions){
      jsonOutput[key]["sample"] = itr.value();
      sampleWithInversions++;
      isInversions = false;
    }

  }

  jsonOutput["metadata"]["arraysize"] = arraySize;
  jsonOutput["metadata"]["file"] = argv[1];
  jsonOutput["metadata"]["numSamples"] = numSamples;
  jsonOutput["metadata"]["samplesWithInversions"] = sampleWithInversions;
  
  cout << jsonOutput.dump(2) << "\n";
}
