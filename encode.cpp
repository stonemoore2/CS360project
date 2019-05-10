#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

string decimal_to_binary(int n){
    string s;
    while(n != 0){
    	s = (n % 2 == 0 ? "0":"1") + s; 
    	n /= 2;
    }
    return s;
}

string char_to_binary(char c){
	int i = c;
	string s = decimal_to_binary(i);
	while (s.length() < 8)
		s = "0" + s;
	return s;
}

vector<string> string_to_binary(string input){
	vector<string> result;
	for (int i = 0; i < input.size(); i++){
		if (i % 2 == 0)
			result.push_back(char_to_binary(input[i]));
		else
			result.back().append(char_to_binary(input[i]));
	}
	if (result.back().size() < 16)
		result.back().append("00000000");
	while (result.size() < 2048)
		result.push_back("0000000000000000");
	return result;
}