#include <string>

using namespace std;

struct instruction{

};

struct declaration{
	int value;
	string destination;
}

struct logicOp{
	string opsymbol, operand1, operand2, destination;
};

struct conditional{
	logicOp condition;
	instruction[] fulfilled;
};

struct for_loop{
	declaration initialize;
	logicOp condition, increment;
	instruction[] fulfilled;
}

struct while_loop{
	conditional loop;
}