#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

string toBinary(int n){
    string r;
    while(n!=0){
    	r=(n%2==0 ?"0":"1")+r; 
    	n/=2;
    }
    return r;
}

string char_to_binary(char c){
	int i = c;
	string s = toBinary(i);
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
	return result;
}

//int main(){
	/*char n;
	cin.get(n);
	cout << char_to_binary(n);*/

	/*vector<string> test = string_to_binary("I'm testing");
	for (int i = 0; i < test.size(); i++)
		cout << test[i] << " ";*/
//}
