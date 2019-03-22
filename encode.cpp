#include <string>
#include <fstream>
#include <iostream>
#include <vector>

//#include <sstream>

using namespace std;

//string inpt = "int main(){\nint a[5]={16,2,77,40,5};\nint max=total(a);\n}";

string char_to_binary(char c){
	if (c == ' ')
		return "00100000";
	string c2(1, c);
	ifstream infile("ascii_table.txt");
	string inp, outp;
	while (infile >> inp >> outp){
		if (inp == c2){
			return outp;
		}
	}
	return "error";
}

/*string char_to_binary(char c){
	string c2(1, c);
	ifstream infile("ascii_table.txt");
	string line;
	while(getline(infile, line)){
		istringstream iss(line);
		string inp, outp;
		if (!(iss >> inp))
			break;
		if (inp == c2){
			getline(infile, line);
			istringstream iss2(line);
			iss2 >> outp;
			return outp;
		}
	}
	return "error";
}*/

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

int main(){
	/*char n;
	cin.get(n);
	cout << char_to_binary(n);*/

	vector<string> test = string_to_binary("I'm testing");
	for (int i = 0; i < test.size(); i++)
		cout << test[i] << " ";
}