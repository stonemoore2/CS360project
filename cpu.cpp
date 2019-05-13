#include <string>

using namespace std;

class CPU{
	public:
		string rax, rdi, rdx, rbp, rsp, eax, edi, pc, page_table, EFLAGS;
		void execute_code(vector<string> ac);
		void execute_step(string as);
		//string decode(string input);
} cpu_;

//evaluate address in brackets
int pemdas_eval(string to_eval){
	vector<string> parsed = parse(to_eval);
	for (unsigned int i = 0; i < parsed.size() - 2; i += 2){
		int term1, term2;
		if (parsed[i] == "rbp")
			term1 = binary_to_decimal(cpu_.rbp);
		else if (parsed[i] == "rsp")
			term1 = binary_to_decimal(cpu_.rsp);
		else if (parsed[i] == "rdi")
			term1 = binary_to_decimal(cpu_.rdi);
		else if (parsed[i] == "rax")
			term1 = binary_to_decimal(cpu_.rax);
		else if (parsed[i] == "rdx")
			term1 = binary_to_decimal(cpu_.rdx);
		else if (parsed[i] == "edi")
			term1 = binary_to_decimal(cpu_.edi);
		else if (parsed[i] == "eax")
			term1 = binary_to_decimal(cpu_.eax);
		else
			term1 = stoi(parsed[i]);

		if (parsed[i+2] == "rbp")
			term2 = binary_to_decimal(cpu_.rbp);
		else if (parsed[i+2] == "rsp")
			term2 = binary_to_decimal(cpu_.rsp);
		else if (parsed[i+2] == "rdi")
			term2 = binary_to_decimal(cpu_.rdi);
		else if (parsed[i+2] == "rax")
			term2 = binary_to_decimal(cpu_.rax);
		else if (parsed[i+2] == "rdx")
			term2 = binary_to_decimal(cpu_.rdx);
		else if (parsed[i+2] == "edi")
			term2 = binary_to_decimal(cpu_.edi);
		else if (parsed[i+2] == "eax")
			term2 = binary_to_decimal(cpu_.eax);
		else
			term2 = stoi(parsed[i+2]);


		if (parsed[i+1] == "*"){
			parsed[i+2] = to_string(term1 * term2);
			parsed.erase(parsed.begin() + i, parsed.begin() + i + 2);
		}
		else if (parsed[i+1] == "/"){
			parsed[i+2] = to_string(term1 / term2);
			parsed.erase(parsed.begin() + i, parsed.begin() + i + 2);
		}
	}

	for(unsigned int i = 0; i < parsed.size() - 2; i += 2){
		if (parsed[i+1] == "+")
			parsed[i+2] = to_string(stoi(parsed[i]) + stoi(parsed[i+2]));
		else if (parsed[i+1] == "-")
			parsed[i+2] = to_string(stoi(parsed[i]) - stoi(parsed[i+2]));
	}
	return stoi(parsed.back());
}

int arith_logic(string op1_type, string op1, string op2_type, string op2, string operation, int & address1){
	int address2;
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
		if (op1 == "rax")
			address1 =  stoi(cpu_.rax);
		else if (op1 == "rdi")
			address1 =  stoi(cpu_.rdi);
		else if (op1 == "rdx")
			address1 =  stoi(cpu_.rdx);
		else if (op1 == "rbp")
			address1 =  stoi(cpu_.rbp);
		else if (op1 == "rsp")
			address1 =  stoi(cpu_.rsp);
		else if (op1 == "eax")
			address1 =  stoi(cpu_.eax);
		else if (op1 == "edi")
			address1 =  stoi(cpu_.edi);
		else
			address1 = pemdas_eval(op1);

		//op1 = element located at address1 in memory

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
		if (op2 == "rax")
			address2 =  stoi(cpu_.rax);
		else if (op2 == "rdi")
			address2 =  stoi(cpu_.rdi);
		else if (op2 == "rdx")
			address2 =  stoi(cpu_.rdx);
		else if (op2 == "rbp")
			address2 =  stoi(cpu_.rbp);
		else if (op2 == "rsp")
			address2 =  stoi(cpu_.rsp);
		else if (op2 == "eax")
			address2 =  stoi(cpu_.eax);
		else if (op2 == "edi")
			address2 =  stoi(cpu_.edi);
		else
			address2 = pemdas_eval(op2);

		//op2 = element located at address2 in memory

	}

	//if op2_type = 2 (immediate) it already contains the necessary value; 

	if (operation == "add")
		return stoi(op1) + stoi(op2);
	else if (operation == "sub")
		return stoi(op1) - stoi(op2);
	else if (operation == "cmp"){
		if (stoi(op1) < stoi(op2))
			return 1;
		else if (stoi(op1) > stoi(op2))
			return 2;
		else
			return 0;
	}
	else //mov
		return stoi(op2); //memory shenanigans
}

void jump(string operation, string label){
	if (operation == "je" && cpu_.EFLAGS != "0000000000000000")
		return;
	else if(operation == "jg" && cpu_.EFLAGS != "0000000000000010")
		return;
	else if(operation == "jge" && cpu_.EFLAGS != "0000000000000010" && cpu_.EFLAGS != "0000000000000000")
		return;
	else if(operation == "jl" && cpu_.EFLAGS != "0000000000000001")
		return;
	else if(operation == "jle" && cpu_.EFLAGS != "0000000000000001" && cpu_.EFLAGS != "0000000000000000")
		return;
	else if(operation == "jne" && cpu_.EFLAGS == "0000000000000000")
		return;

	//go to label

}


void CPU::execute_code (vector<string> ac){
	for (int i = 0; i < ac.size(); i++)
		execute_step(ac[i]);
	return;
}


void CPU::execute_step(string as){
	string op1_type, op1, op2_type, op2, operation;
	vector<string> split_ops = split_on_char (as, ' ');
	operation = split_ops.at(0);

	//arith/logic
	if (operation == "mov" || operation == "add" || operation == "sub" || operation == "cmp"){
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
				op2 = split_ops.at(2);
			}
			op2 = split_ops.at(2);
		}
		int store_address = 0;
		string binary_result = decimal_to_binary(arith_logic(op1_type, op1, op2_type, op2, operation, store_address));
		if (operation == "cmp"){
			cpu_.EFLAGS = binary_result;
			while(cpu_.EFLAGS.size() < 16)
				cpu_.EFLAGS = "0" + cpu_.EFLAGS;
			return;
		}

		if (op1_type == "0"){
			if (split_ops.at(1) == "rax")
				rax = binary_result;
			else if (split_ops.at(1) == "rdi")
				rdi = binary_result;
			else if (split_ops.at(1) == "rdx")
				rdx = binary_result;
			else if (split_ops.at(1) == "rbp")
				rbp = binary_result;
			else if (split_ops.at(1) == "rsp")
				rsp = binary_result;
			else if (split_ops.at(1) == "eax")
				eax = binary_result;
			else if (split_ops.at(1) == "edi")
				edi = binary_result;
		}
		else{

			//store_address in memory = binary_result;

		}
	}

	//jump
	else if(as[0] == 'j'){
		jump(operation, split_ops.at(1));
	}


	return;
}