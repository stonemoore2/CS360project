#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

struct instruction{
	string code;
};

struct arith_logic_op: public instruction{
	string opsymbol, operand1, operand2;
};

struct declaration: public instruction{
	struct arith_logic_op value;
	string destination;
};

struct conditional_: public instruction{
	struct arith_logic_op condition;
	vector<instruction> fulfilled;
};

struct for_loop: public instruction{
	struct declaration initialize;
	struct arith_logic_op condition, increment;
	vector<instruction> fulfilled;
};

struct while_loop: public instruction{
	struct conditional_ loop;
};


vector<string> split(string str, char delimeter){
    stringstream ss(str);
    string item;
    vector<string> split_strings;
    while (getline(ss, item, delimeter)){
       split_strings.push_back(item);
    }
    return split_strings;
}

//add checks for {}
struct instruction parse(string prog){
	for (int i = 0; i < prog.length(); i++){
		if (prog[i] == '=' && prog.substr(i,2) != "=="){
			struct declaration dcl;
			dcl.destination = prog.substr(0,i);
			dcl.value = parse(prog.substr(i+1));
			return dcl;
		}
		else if (prog[i] == '+' || prog[i] == '-' || prog[i] == '*' || prog[i] == '/'){
			struct arith_logic_op alo;
			alo.operand1 = prog.substr(0,i);
			alo.operand2 = prog.substr(i+1);
			alo.opsymbol = prog.substr(i,1);
			return alo;
		}
	}
	return NULL;
}

string instruction_to_assembly(struct instruction){
	
}