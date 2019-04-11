#include "memory.cpp"
#include "encode.cpp"
#include "translator.cpp"
//#include "structs.cpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>

using namespace std;

string file_to_string(ifstream& in){
    stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

ifstream infile("input.txt");
string code = file_to_string(infile);

memory external = memory(2048);
vector<string> binary = string_to_binary(code);

/*
vector<string> lines = split(code, ';');
//vector<string> instr;

//for (string x: lines){
//	instr.push_back(parse(x));
}
*/
int main(){
  cout<<"TRANSLATED SOURCE CODE"<<endl;
  string assembly = translate(code);
  cout<<assembly<<endl<<"MEMORY"<<endl;
  
	for (int j = 0; j < binary.size(); j++){
		external.setword(j, binary[j]);
	}
		cout << "| Word | Data |" << "\n";
	for (int i = 0; i < external.getsize(); i++){
	  printf ("|  %i   | %s |\n", i, external.getword(i).c_str());
    //code doesn't work on my mac without the .c_str() at the end. If this causes
    //a problem removing it should hopefully fix it - Emile
}

}
