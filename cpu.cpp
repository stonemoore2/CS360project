#include <string>

using namespace std;

/*class CPU{
	public:
		string rax, rdi, rdx, rbp, rsp, eax, edi, pc, page_table;
		string decode(string input);
};*/

int arith_logic(string op1_type, string op1, string op2_type, string op2, string operation){
	if(op1_type == "0"){
		if (op1 == "rax")
			op1 =  cpu_.rax;
		else if (op1 == "rdi")
			op1 =  cpu_.rdi;
		else if (op1 == "rdx")
			op1 =  cpu_.rdx;
		else if (op1 == "rbp")
			op1 =  cpu_.rbp;
		else if (op1 == "rsp")
			op1 =  cpu_.rsp;
		else if (op1 == "eax")
			op1 =  cpu_.eax;
		else if (op1 == "edi")
			op1 =  cpu_.edi;
	}
	else{
		//memory address shenanigans
	}
	if(op2_type == "0"){
		if (op2 == "rax")
			op2 =  cpu_.rax;
		else if (op2 == "rdi")
			op2 =  cpu_.rdi;
		else if (op2 == "rdx")
			op2 =  cpu_.rdx;
		else if (op2 == "rbp")
			op2 =  cpu_.rbp;
		else if (op2 == "rsp")
			op2 =  cpu_.rsp;
		else if (op2 == "eax")
			op2 =  cpu_.eax;
		else if (op2 == "edi")
			op2 =  cpu_.edi;
	}
	else if(op2_type == "1"){
		//memory address shenanigans
	}
	if (operation == "add")
		return stoi(op1) + stoi(op2);
	else if (operation == "sub")
		return stoi(op1) - stoi(op2);
	else if (operation == "cmp")
		return stoi(op1) > stoi(op2);
	else{ //mov
		//memory shenanigans
		return 0;
	}
}

void jump(string label){

}



void execute_code(vector<string> assembly){
	for (int i = 0; i < assembly.size(); i++){
		string op1_type, op1, op2_type, op2, operation;
		string current = assembly[i];
		if (current.substr(0,3) == "mov" || current.substr(0,3) == "add" || current.substr(0,3) == "sub" || current.substr(0,3) == "cmp"){
			vector<string> split_ops = split_on_char (current, ' ');
			operation = split_ops.at(0);
			if (split_ops.at(1)[0] == '['){
				op1_type = "1";
				op1 = split_ops.at(1).substr(1);
				op1.pop_back();
			}
			else{
				op1_type = "0";
				op1 = split_ops.at(1);
			}
			if (split_ops.at(2)[0] == '['){
				op2_type = "1";
				op2 = split_ops.at(2).substr(1);
				op2.pop_back();
			}
			else{
				if(split_ops.at(2) == "rax" || split_ops.at(2) == "rdi" || split_ops.at(2) == "rdx" || split_ops.at(2) == "rbp" || split_ops.at(2) == "rsp" || split_ops.at(2) == "eax" || split_ops.at(2) == "edi"){
					op2_type = "0";
				}
				else{
					op2_type = "2";
				}
				op2 = split_ops.at(2);
			}
		}
	}
}