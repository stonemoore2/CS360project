#include "memory.cpp"
#include "encode.cpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

string file_to_string(ifstream& in){
    stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

ifstream infile("input.txt");
string code = file_to_string(infile);

int main(){
	
}