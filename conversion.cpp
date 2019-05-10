#include <string>
#include <sstream>
#include <vector>

using namespace std;

vector<string> split_on_char(string to_split, char deliniator){
	stringstream sstr(to_split);
	string segment;
	vector<string> result;
	while(getline(sstr, segment, deliniator))
		result.push_back(segment);
	return result;
}

string register_encoder(string reg){
	if (reg == "rbp,")
		return "0000";
	else if (reg == "rsp,")
		return "0001";
	else if (reg == "rdi,")
		return "0010";
	else if (reg == "rax,")
		return "0011";
	else if (reg == "rdx")
		return "0100";
	else if (reg == "edi,")
		return "0101";
	else if (reg == "eax,")
		return "0110";
	else
		return "error";
}

string operand_encoder(string operand){
	string result;
	if (register_encoder(operand) != "error"){
		result += "1"	;						// 1 for registers
		result += register_encoder(operand);
		result += "000000000000";
	}
	else{
		result += "0";									//0 for immediates
		int int_imm	 = stoi(operand);			
		string str_imm = decimal_to_binary(int_imm);
		while (str_imm.size() < 16)
			str_imm = "0" + str_imm;
		result += str_imm;
	}
	return result;
}

string assembly_line_to_machine (string input){
	string result;
	vector<string> split_assembly = split_on_char(input, ' ');
	if (split_assembly[0] == "add"){
		result += "00000";
		result += register_encoder(split_assembly[1]);
		result += operand_encoder(split_assembly[2]);
		result += "000000";
	}
	else if (split_assembly[0] == "sub"){
		result += "00001";
		result += register_encoder(split_assembly[1]);
		result += operand_encoder(split_assembly[2]);
		result += "000000";
	}
	else if (split_assembly[0] == "cmp"){
		result += "00010";
		result += register_encoder(split_assembly[1]);
		result += operand_encoder(split_assembly[2]);
		result += "000000";
	}
	else if (split_assembly[0] == "cdqe"){
		result += "00011000000000000000000000000000";
	}
	else if (split_assembly[0] == "call"){
		result += "11000";
	}
	else if (split_assembly[0] == "ret"){
		result += "11001";
	}
	else if (split_assembly[0] == "push"){
		result += "10000";
		result += operand_encoder(split_assembly[1]);
		result += "0000000000";
	}
	else if (split_assembly[0] == "pop"){
		result += "10001";
		result += register_encoder(split_assembly[1]);
		result += "00000000000000000000000";
	}
	else if (split_assembly[0] == "je"){
		result += "01000";
		//label
	}
	else if (split_assembly[0] == "jg"){
		result += "01001";
	}
	else if (split_assembly[0] == "jge"){
		result += "01010";
	}
	else if (split_assembly[0] == "jl"){
		result += "01011";
	}
	else if (split_assembly[0] == "jle"){
		result += "01100";
	}
	else if (split_assembly[0] == "jmp"){
		result += "01101";
	}
	else if (split_assembly[0] == "jne"){
		result += "01110";
	}
	else if (split_assembly[0] == "mov"){
		result += "00100";
		result += register_encoder(split_assembly[1]);
		result += operand_encoder(split_assembly[2]);
		result += "000000";
	}
	else if (split_assembly[0] == "leave"){
		result += "11010000000000000000000000000000";
	}
	else if (split_assembly[0] == "lea"){
		result += "10010";
		result += register_encoder(split_assembly[1]);
	}
	else
		result += "error";
	return result;
}

/*string machine_to_assembly (string input){
	string result, current_substr;
	int position;
	current_substr = input(0,5);
	position = 5;
		//if ( == "00000":
			result += "add";

	}*/