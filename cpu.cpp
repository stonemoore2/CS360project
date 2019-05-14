#include <string>

using namespace std;

class CPU{
	public:
		int pc = -1;		
		int rax, rdi, rdx, rbp = 50, rsp = 50, eax, edi, page_table, EFLAGS;
		vector <int> call_stack = vector <int> (50, 0);

		void execute_code(vector<string> ac);
		void execute_step(string as);
		//string decode(string input);
} cpu_;

//evaluate address in brackets
int pemdas_eval(string to_eval){
	vector<string> parsed = parse(to_eval);
	for (unsigned int i = 0; i < parsed.size(); i++){
		if ((isdigit(parsed.at(i)[0])) && (i == 0 || (i > 0 && (parsed[i-1] == "-" || parsed[i-1] == "+")))){
			int a = stoi(parsed[i]) / 4;
			parsed[i] = to_string(a);			
		}
	}

	for (unsigned int i = 0; i < parsed.size() - 2; i += 2){
		int term1, term2;
		if (parsed[i] == "rbp")
			term1 = cpu_.rbp;
		else if (parsed[i] == "rsp")
			term1 = cpu_.rsp;
		else if (parsed[i] == "rdi")
			term1 = cpu_.rdi;
		else if (parsed[i] == "rax")
			term1 = cpu_.rax;
		else if (parsed[i] == "rdx")
			term1 = cpu_.rdx;
		else if (parsed[i] == "edi")
			term1 = cpu_.edi;
		else if (parsed[i] == "eax")
			term1 = cpu_.eax;
		else
			term1 = stoi(parsed[i]);


		if (parsed[i+2] == "rbp")
			term2 = cpu_.rbp;
		else if (parsed[i+2] == "rsp")
			term2 = cpu_.rsp;
		else if (parsed[i+2] == "rdi")
			term2 = cpu_.rdi;
		else if (parsed[i+2] == "rax")
			term2 = cpu_.rax;
		else if (parsed[i+2] == "rdx")
			term2 = cpu_.rdx;
		else if (parsed[i+2] == "edi")
			term2 = cpu_.edi;
		else if (parsed[i+2] == "eax")
			term2 = cpu_.eax;
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
		int term1, term2;
		if (parsed[i] == "rbp")
			term1 = cpu_.rbp;
		else if (parsed[i] == "rsp")
			term1 = cpu_.rsp;
		else if (parsed[i] == "rdi")
			term1 = cpu_.rdi;
		else if (parsed[i] == "rax")
			term1 = cpu_.rax;
		else if (parsed[i] == "rdx")
			term1 = cpu_.rdx;
		else if (parsed[i] == "edi")
			term1 = cpu_.edi;
		else if (parsed[i] == "eax")
			term1 = cpu_.eax;
		else
			term1 = stoi(parsed[i]);


		if (parsed[i+2] == "rbp")
			term2 = cpu_.rbp;
		else if (parsed[i+2] == "rsp")
			term2 = cpu_.rsp;
		else if (parsed[i+2] == "rdi")
			term2 = cpu_.rdi;
		else if (parsed[i+2] == "rax")
			term2 = cpu_.rax;
		else if (parsed[i+2] == "rdx")
			term2 = cpu_.rdx;
		else if (parsed[i+2] == "edi")
			term2 = cpu_.edi;
		else if (parsed[i+2] == "eax")
			term2 = cpu_.eax;
		else
			term2 = stoi(parsed[i+2]);

		if (parsed[i+1] == "+")
			parsed[i+2] = to_string(term1 + term2);
		else if (parsed[i+1] == "-")
			parsed[i+2] = to_string(term1 - term2);
	}
	return stoi(parsed.back());
}

int arith_logic(int op1_type, string inp1, int op2_type, string inp2, string operation, int & address1){
	int address2;
	int ope1, ope2;
	if(op1_type == 0){
		if (inp1 == "rax")
			ope1 =  cpu_.rax;
		else if (inp1 == "rdi")
			ope1 =  cpu_.rdi;
		else if (inp1 == "rdx")
			ope1 =  cpu_.rdx;
		else if (inp1 == "rbp")
			ope1 =  cpu_.rbp;
		else if (inp1 == "rsp")
			ope1 =  cpu_.rsp;
		else if (inp1 == "eax")
			ope1 =  cpu_.eax;
		else if (inp1 == "edi")
			ope1 =  cpu_.edi;
	}

	

	else{
		//memory address shenanigans
		if (inp1 == "rax")
			address1 =  cpu_.rax;
		else if (inp1 == "rdi")
			address1 =  cpu_.rdi;
		else if (inp1 == "rdx")
			address1 =  cpu_.rdx;
		else if (inp1 == "rbp")
			address1 =  cpu_.rbp;
		else if (inp1 == "rsp")
			address1 =  cpu_.rsp;
		else if (inp1 == "eax")
			address1 =  cpu_.eax;
		else if (inp1 == "edi")
			address1 =  cpu_.edi;
		else
			address1 = pemdas_eval(inp1);

		
		//op1 = element located at address1 in memory
		ope1 = cpu_.call_stack[address1];

		
	}



	if(op2_type == 0){
		if (inp2 == "rax")
			ope2 =  cpu_.rax;
		else if (inp2 == "rdi")
			ope2 =  cpu_.rdi;
		else if (inp2 == "rdx")
			ope2 =  cpu_.rdx;
		else if (inp2 == "rbp")
			ope2 =  cpu_.rbp;
		else if (inp2 == "rsp")
			ope2 =  cpu_.rsp;
		else if (inp2 == "eax")
			ope2 =  cpu_.eax;
		else if (inp2 == "edi")
			ope2 =  cpu_.edi;
	}
	else if(op2_type == 1){
		//memory address shenanigans
		if (inp2 == "rax")
			address2 = cpu_.rax;
		else if (inp2 == "rdi")
			address2 = cpu_.rdi;
		else if (inp2 == "rdx")
			address2 = cpu_.rdx;
		else if (inp2 == "rbp")
			address2 = cpu_.rbp;
		else if (inp2 == "rsp")
			address2 = cpu_.rsp;
		else if (inp2 == "eax")
			address2 = cpu_.eax;
		else if (inp2 == "edi")
			address2 = cpu_.edi;
		else
			address2 = pemdas_eval(inp2);

		//op2 = element located at address2 in memory
		ope2 = cpu_.call_stack[address2];

	}

	//immediate
	else
		ope2 = stoi(inp2);



	if (operation == "add")
		return ope1 + ope2;
	else if (operation == "sub")
		return ope1 - ope2;
	else if (operation == "cmp"){
		if (ope1 < ope2)
			return 1;
		else if (ope1 > ope2)
			return 2;
		else
			return 0;
	}
	else{ //mov
		return ope2; //memory shenanigans
	}
}

void jump(string operation, string label){
	if (operation == "je" && cpu_.EFLAGS != 0)
		return;
	else if(operation == "jg" && cpu_.EFLAGS != 2)
		return;
	else if(operation == "jge" && cpu_.EFLAGS != 2 && cpu_.EFLAGS != 0)
		return;
	else if(operation == "jl" && cpu_.EFLAGS != 1)
		return;
	else if(operation == "jle" && cpu_.EFLAGS != 1 && cpu_.EFLAGS != 0)
		return;
	else if(operation == "jne" && cpu_.EFLAGS == 0)
		return;

	for (int i = 0; i < label_vec.size(); i++){
		if (label_vec[i] == label)
			cpu_.pc = i;
	}	
}


void CPU::execute_code (vector<string> ac){
	for (unsigned int i = 0; i < ac.size(); i++)
		execute_step(ac[i]);
	return;
}


void CPU::execute_step(string as){
	int op1_type, op2_type;
	string op1, op2, operation;
	vector<string> split_ops = split_on_char (as, ' ');
	operation = split_ops.at(0);

	//arith/logic
	if (operation == "mov" || operation == "add" || operation == "sub" || operation == "cmp"){
		if (split_ops.at(1)[0] == '['){
			op1_type = 1;
			op1 = split_ops.at(1).substr(1);
			op1.pop_back();
		}
		else{
			op1_type = 0;
			op1 = split_ops.at(1);
		}
		op1.pop_back();

		if (split_ops.at(2)[0] == '['){
			op2_type = 1;
			op2 = split_ops.at(2).substr(1);
			op2.pop_back();
		}
		else{
			op2 = split_ops.at(2);
			if(split_ops.at(2) == "rax" || split_ops.at(2) == "rdi" || split_ops.at(2) == "rdx" || split_ops.at(2) == "rbp" || split_ops.at(2) == "rsp" || split_ops.at(2) == "eax" || split_ops.at(2) == "edi"){
				op2_type = 0;
			}
			else{
				op2_type = 2;
				if (op1 == "rbp" || op1 == "rsp")
					op2 = to_string(stoi(op2) / 4);
			}	
		}


		int store_address = 0;
		int result = arith_logic(op1_type, op1, op2_type, op2, operation, store_address);

		if (operation == "cmp"){
			cpu_.EFLAGS = result;
			return;
		}


		if (op1_type == 0){
			if (op1 == "rax")
				cpu_.rax = result;
			else if (op1 == "rdi")
				cpu_.rdi = result;
			else if (op1 == "rdx")
				cpu_.rdx = result;
			else if (op1 == "rbp")
				cpu_.rbp = result;
			else if (op1 == "rsp")
				cpu_.rsp = result;
			else if (op1 == "eax")
				cpu_.eax = result;
			else if (op1 == "edi")
				cpu_.edi = result;
		}
		else{
			int addr = pemdas_eval(op1);
			call_stack[addr] = result;
			//if (addr < )
		}
	}

	//cdqe
	else if(operation == "cdqe"){

	}

	//call
	else if(operation == "call"){
		//push address on stack
		cpu_.rsp -= 1;
		cpu_.call_stack[cpu_.rsp] = cpu_.pc - 1;
		//jump to function
		for (int i = 0; i < label_vec.size(); i++){
			if (label_vec[i] == split_ops[1])
				cpu_.pc = i;
		}	 
	}

	//return
	else if(operation == "ret"){
		int addr = call_stack.back();
		call_stack.pop_back();
		//return to caller
	}

	//push
	else if(operation == "push"){
		cpu_.rsp -= 1;
		if (split_ops.at(1) == "rax")
			cpu_.call_stack[cpu_.rsp] = rax;
		else if (split_ops.at(1) == "rdi")
			cpu_.call_stack[cpu_.rsp] = rdi;
		else if (split_ops.at(1) == "rdx")
			cpu_.call_stack[cpu_.rsp] = rdx;
		else if (split_ops.at(1) == "rbp")
			cpu_.call_stack[cpu_.rsp] = rbp;
		else if (split_ops.at(1) == "rsp")
			cpu_.call_stack[cpu_.rsp] = rsp;
		else if (split_ops.at(1) == "eax")
			cpu_.call_stack[cpu_.rsp] = eax;
		else if (split_ops.at(1) == "edi")
			cpu_.call_stack[cpu_.rsp] = edi;
		

	}

	//pop
	else if(operation == "pop"){
		int to_pop = call_stack[cpu_.rsp];
		call_stack[cpu_.rsp] = 0;
		cpu_.rsp += 1;
		if (split_ops.at(1) == "rax")
			cpu_.rax = to_pop;
		else if (split_ops.at(1) == "rdi")
			cpu_.rdi = to_pop;
		else if (split_ops.at(1) == "rdx")
			cpu_.rdx = to_pop;
		else if (split_ops.at(1) == "rbp")
			cpu_.rbp = to_pop;
		else if (split_ops.at(1) == "rsp")
			cpu_.rsp = to_pop;
		else if (split_ops.at(1) == "eax")
			cpu_.eax = to_pop;
		else if (split_ops.at(1) == "edi")
			cpu_.edi = to_pop;
	}

	//jump
	else if(as[0] == 'j'){
		jump(operation, split_ops.at(1));
	}

	//leave
	else if(operation == "leave"){

	}

	//load effective address
	else if(operation == "lea"){

	}

	return;
}