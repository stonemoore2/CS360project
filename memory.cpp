#include <string>
#include <vector>

using namespace std;

class memory{
	vector<string> bits;
public:
	memory(int n);
	string getword(int address);
	void setword(int address, string new_word);
	int getsize();
};

memory::memory(int n){
	bits.resize(n);
}

string memory::getword(int address){
	return bits[address];
}

void memory::setword(int address, string new_word){
	bits[address] = new_word;
}

int memory::getsize(){
	return bits.size();
}