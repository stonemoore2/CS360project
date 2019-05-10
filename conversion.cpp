#include <string>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

vector<int> calls;

class CPU{
	public:
		string rax = "01111111111", rdi = "10111111111", rdx = "11011111111", rbp = "11101111111", 
rsp = "11110111111", eax = "11111011111", edi = "11111101111", pc, page_table;
		string decode(string input);
};
CPU cpu_;

int vec_find(vector<string> vec, string to_find){
	for (unsigned int i = 0; i < vec.size(); i++)
		if (vec[i] == to_find)
			return i;
	return -1;
}

vector<string> split_on_char(string to_split, char deliniator){
    vector<string> result;
    if (to_split.find(deliniator) == string::npos){
		result.push_back(to_split);
		return result;
	}
	stringstream sstr(to_split);
	string segment;
	while(getline(sstr, segment, deliniator))
		result.push_back(segment);
	return result;
}

string split_on_char_with_delin(string to_split, char deliniator){
    if (to_split.find(deliniator) == string::npos)
		return to_split;
	stringstream sstr(to_split);
	string segment, result;
	while(getline(sstr, segment, deliniator)){
		result += segment;
		result += ',';
		result += deliniator;
		result += ',';
	}
	result.pop_back();
	result.pop_back();
	result.pop_back();
	return result;
}

vector<string> parse(string input){
	vector<string> result;
	input = split_on_char_with_delin(input, '+');
	input = split_on_char_with_delin(input, '-');
	input = split_on_char_with_delin(input, '*');
	input = split_on_char_with_delin(input, '/');
	result = split_on_char(input, ',');
	return result;
}

int binary_to_decimal(string binary){
	int result = 0, i = 1;
	if (binary[binary.size()-1] == '1')
		result = 1;
		binary.pop_back();
	while (binary != ""){
		if (binary[binary.size()-1] == '1')
			result += (pow(2.0,i));
		binary.pop_back();
		i++;
	}
	return result;
}

string eval_stack(vector<string> stack){
    if (stack.size() == 1){
		if (stack[0] == "rbp]")
			return cpu_.rbp;
		else if (stack[0] == "rsp]")
			return cpu_.rsp;
		else if (stack[0] == "rdi]")
			return cpu_.rdi;
		else if (stack[0] == "rax]")
			return cpu_.rax;
		else if (stack[0] == "rdx]")
			return cpu_.rdx;
		else if (stack[0] == "edi]")
			return cpu_.edi;
		else if (stack[0] == "eax]")
			return cpu_.eax;
		else
			return "error";
	}
	for (unsigned int i = 0; i < stack.size() - 2; i += 2){
		int op1, op2;
		if (stack[i] == "rbp")
			op1 = binary_to_decimal(cpu_.rbp);
		else if (stack[i] == "rsp")
			op1 = binary_to_decimal(cpu_.rsp);
		else if (stack[i] == "rdi")
			op1 = binary_to_decimal(cpu_.rdi);
		else if (stack[i] == "rax")
			op1 = binary_to_decimal(cpu_.rax);
		else if (stack[i] == "rdx")
			op1 = binary_to_decimal(cpu_.rdx);
		else if (stack[i] == "edi")
			op1 = binary_to_decimal(cpu_.edi);
		else if (stack[i] == "eax")
			op1 = binary_to_decimal(cpu_.eax);
		else
			op1 = stoi(stack[i]);

		if (stack[i+2] == "rbp")
			op2 = binary_to_decimal(cpu_.rbp);
		else if (stack[i+2] == "rsp")
			op2 = binary_to_decimal(cpu_.rsp);
		else if (stack[i+2] == "rdi")
			op2 = binary_to_decimal(cpu_.rdi);
		else if (stack[i+2] == "rax")
			op2 = binary_to_decimal(cpu_.rax);
		else if (stack[i+2] == "rdx")
			op2 = binary_to_decimal(cpu_.rdx);
		else if (stack[i+2] == "edi")
			op2 = binary_to_decimal(cpu_.edi);
		else if (stack[i+2] == "eax")
			op2 = binary_to_decimal(cpu_.eax);
		else
			op2 = stoi(stack[i+2]);

		if (stack[i+1] == "+")
			stack[i+2] = to_string(op1 + op2);
		else if (stack[i+1] == "-")
			stack[i+2] = to_string(op1 - op2);
		else if (stack[i+1] == "*")
			stack[i+2] = to_string(op1 * op2);
		else if (stack[i+1] == "/")
			stack[i+2] = to_string(op1 / op2);
	}
	return decimal_to_binary(stoi(stack[stack.size()-1]));
}

string operand1_encoder(string operand){
	if (operand[operand.size()-1] == ',')
		operand.pop_back();
	if (operand[0] == '['){
		operand = operand.substr(1,operand.size()-1);
		vector<string> parse_stack = parse(operand);
		string evaluated = eval_stack(parse_stack);
		while (evaluated.size() < 12)
			evaluated = "0" + evaluated;
		return "1" + evaluated;							//1 for addresses
	}
	else if (operand == "rbp")							//0 for registers
		return "0000000000000";
	else if (operand == "rsp")
		return "0000100000000";
	else if (operand == "rdi")
		return "0001000000000";
	else if (operand == "rax")
		return "0001100000000";
	else if (operand == "rdx")
		return "0010000000000";
	else if (operand == "edi")
		return "0010100000000";
	else if (operand == "eax")
		return "0011000000000";
	else
		return "error";
}

string operand2_encoder(string operand){
	string result;
	if (operand1_encoder(operand) != "error"){
		result += "0"	;								//0X for non-immediates
		result += operand1_encoder(operand);	
	}
	else{
		result += "10";									//10 for immediates
		int int_imm	 = stoi(operand);			
		string str_imm = decimal_to_binary(int_imm);
		while (str_imm.size() < 12)
			str_imm = "0" + str_imm;
		result += str_imm;
	}
	return result;
}

string label_encoder(string label, vector<string> label_vec, int p){
	if (vec_find(label_vec, label) != -1){
		int jump = (vec_find(label_vec, label)) * 4;
		string bin_jump = decimal_to_binary(jump);
		while (bin_jump.size() < 11)
			bin_jump = "0" + bin_jump;
		return bin_jump + "0000000000000000";
	}
	else
		return "error";
}

string assembly_line_to_machine (string input, vector<string> av, vector<string> lv, int pos){
	string result;
	vector<string> split_assembly = split_on_char(input, ' ');
	if (split_assembly[0] == "add"){
		result += "00000";
		result += operand1_encoder(split_assembly[1]);
		result += operand2_encoder(split_assembly[2]);
	}
	else if (split_assembly[0] == "sub"){
		result += "00001";
		result += operand1_encoder(split_assembly[1]);
		result += operand2_encoder(split_assembly[2]);
	}
	else if (split_assembly[0] == "cmp"){
		result += "00010";
		result += operand1_encoder(split_assembly[1]);
		result += operand2_encoder(split_assembly[2]);
	}
	else if (split_assembly[0] == "cdqe"){
		result += "00011000000000000000000000000000";
	}
	else if (split_assembly[0] == "call"){
		result += "11000";
		string label = split_assembly[1];
		result += label_encoder(label, lv, pos);
		calls.push_back((pos+1) * 4);
	}
	else if (split_assembly[0] == "ret"){
		result += "11001";
		if (calls.size() != 0){
			string jump = decimal_to_binary(calls.back());
			calls.pop_back();
			while (jump.size() < 11)
				jump = "0" + jump;
			result += jump;
		}
		else
			result += "11111111111";
		result += "0000000000000000";
	}
	else if (split_assembly[0] == "push"){
		result += "10000";
		result += operand2_encoder(split_assembly[1]);
		result += "0000000000000";
	}
	else if (split_assembly[0] == "pop"){
		result += "10001";
		result += operand1_encoder(split_assembly[1]);
		result += "00000000000000";
	}
	else if (split_assembly[0] == "je"){
		result += "01000";
		string label = split_assembly[1];
		result += label_encoder(label, lv, pos);
	}
	else if (split_assembly[0] == "jg"){
		result += "01001";
		string label = split_assembly[1];
		result += label_encoder(label, lv, pos);
	}
	else if (split_assembly[0] == "jge"){
		result += "01010";
		string label = split_assembly[1];
		result += label_encoder(label, lv, pos);
	}
	else if (split_assembly[0] == "jl"){
		result += "01011";
		string label = split_assembly[1];
		result += label_encoder(label, lv, pos);
	}
	else if (split_assembly[0] == "jle"){
		result += "01100";
		string label = split_assembly[1];
		result += label_encoder(label, lv, pos);
	}
	else if (split_assembly[0] == "jmp"){
		result += "01101";
		string label = split_assembly[1];
		result += label_encoder(label, lv, pos);
	}
	else if (split_assembly[0] == "jne"){
		result += "01110";
		string label = split_assembly[1];
		result += label_encoder(label, lv, pos);
	}
	else if (split_assembly[0] == "mov"){
		result += "00100";
		result += operand1_encoder(split_assembly[1]);
		result += operand2_encoder(split_assembly[2]);
	}
	else if (split_assembly[0] == "leave"){
		result += "11010000000000000000000000000000";
	}
	else if (split_assembly[0] == "lea"){
		result += "10010";
		result += operand1_encoder(split_assembly[1]);
		result += operand2_encoder(split_assembly[2]);
		/*types:	00 - register
					01 - register content
					10 - immediate
		*/
	}
	else
		result += "error";
	return result;
}

vector<string> label_assign(vector<string>& input){
	vector <string> labels(input.size());
	unsigned int i = 0;
	while (i < input.size()){
		if (input[i].find(":") != string::npos){
			labels[i] = input[i];
			labels[i].pop_back();
			input.erase(input.begin() + i);
		}
		else
			i++;
	}
	return labels;
}

vector<string> assembly_to_machine(string assembly){
	vector<string> machine;
	vector<string> assembly_vec = split_on_char(assembly, '\n');
	vector<string> label_vec = label_assign(assembly_vec);
	for (int i = 0; i < assembly_vec.size(); i++)
		machine.push_back(assembly_line_to_machine(assembly_vec[i], assembly_vec, label_vec, i));
	return machine;
} 