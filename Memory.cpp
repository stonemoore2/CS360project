#include <fstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

// Keeps track of how steps have been taken during step through
int num_of_iteration = 1;

// Keeps track of current address being pointed to by PC, PC initialized as -1 until address of main() is determined
// USING cpu_.pc INSTEAD
//int PC = -1;        

// Keeps track of how many lines machine_code and assembly code needs
int instruction_size = 0;

// Stores assemby code as vector of strings for easy access
vector<string> assembly_mem;

// Stores machine code as vector of string for easy access
vector<string> machine;

// Keeps track of the index value of functions and potential jump points '.L1', '.L2', etc
vector<int> function_index_virt;

// Keeps track of the names of particular index values stored in function_index_virt
vector<string> function_names_virt;

// initiualizes all variables and vectors above function according to the particular "assembly.txt" and "machine_code.txt in file"
// NOTE ******MUST RUN FUNCTION 6 TO WRITE ASSEMBLY AND MACHINE CODE TO FILES*********
void virtual_memory(){

	int instruction_counter = 0;
	int function_counter = -1;

 	ifstream  ifsA("assembly.txt");
 	ifstream  ifsM("machine_code.txt");

 	string instr;
 	string machine_code_ifs;

 	while (ifsM.good()) {

 		getline(ifsM,machine_code_ifs);

 		machine.push_back(machine_code_ifs);
 	}

 	while (ifsA.good()) {

 		getline(ifsA,instr);

 		if (instr.substr(0,1) == "." || instr.substr(0,5) == "total"){

 			cout << instr << endl;
 			function_names_virt.push_back(instr);
 			function_index_virt.push_back(instruction_counter);

 		}

 		else if (instr.substr(0,4) == "main") {

 			cpu_.pc = instruction_counter;
 			cout << instr << endl;

 			function_names_virt.push_back(instr);
 			function_index_virt.push_back(instruction_counter);
 		}

 		else {

 			if (cpu_.pc == instruction_counter) {

 				string PC_string = to_string(instruction_counter);

 				string substring = "";

 				substring = substring + "PC==>(" + PC_string + ") " + instr;

 				string output = "";

 				//string mac = "";

 				//mac = machine[instruction_counter];

 				output = output + substring;

 				cout << output << endl;

	 			instruction_counter++;

	 			assembly_mem.push_back(instr);
 			}

 			else {

 				string PC_string = to_string(instruction_counter);

 				string substring = "";

	 			substring = substring + "     (" + PC_string + ") " + instr;

	 			string output = "";

	 			//string mac = "";

	 			//mac = machine[instruction_counter];

	 			output = output + substring;

	 			cout << output << endl;

	 			instruction_counter++;

	 			assembly_mem.push_back(instr);
	 		}
 		}
 	}

 	instruction_size = instruction_counter;
}

void page_table(){

	int pages = (instruction_size + 1) / 4;

	if (instruction_size % 4 != 0) {

		pages++;
	}

	int page_valid[pages] = {0,0,0,0,0,0,0};

	cout << "///////////////////// PAGE TABLE /////////////////////" << endl;
	cout << "[Valid]\t[Page]\t[Address]" << endl;

	for (int i = 0; i < pages; i++) {

		cout << "[" << page_valid[i] << "]\t" << "[" << i << "]\t" << "Address" << endl; 
	}
}


// WORK IN PROGRESS step_through() will run single lines of code starting from main and update register values as it does so
void step_through() {


	cout << "Step: " << num_of_iteration << endl << endl;




	/*
		[EMI] CACHE OPERATION:
		ATTEMPT TO ACCESS ADDRESS decimal_to_binary(cpu_.pc) PAD ZEROES TO LEFT AS NECESSARY
	*/




	if(assembly_mem.at(cpu_.pc)[0] != 'j' && assembly_mem.at(cpu_.pc).substr(0,4) != "call")
		cpu_.execute_step(assembly_mem[cpu_.pc]);

	num_of_iteration++;
/*
	for (int i = 0; i < function_names_virt.size(); i++) {

		cout << function_names_virt[i] << endl << function_index_virt[i] << endl;
	}
*/
		cout << "//////////////////////// VIRTUAL MEMORY ///////////////////////////" << endl;

		for (int i = 0; i < instruction_size - 1; i++) {

			if (cpu_.pc == i) {

				cout << "PC==> " << "[" << i << "]\t" << machine[i].substr(0,32) << endl;
			}

			else cout << "      " << "[" << i << "]\t" << machine[i].substr(0,32) << endl;
		}

		cout << endl
			 << "///////// CURRENT INSTRUCTION & CURRENT MACHINE CODE///////////////"<< endl;

		cout << "       " << assembly_mem[cpu_.pc] << endl
			 << "       " << machine[cpu_.pc] << endl;

		cout << endl
			 << "///////////////////// REGISTER VALUES /////////////////////////////"<< endl;

		cout << "PC:  " << cpu_.pc << endl
			 << "rbp: " << cpu_.rbp << endl
			 << "rsp: " << cpu_.rsp << endl
			 << "rdi: " << cpu_.rdi << endl
			 << "rax: " << cpu_.rax << endl
			 << "rdx: " << cpu_.rdx << endl
			 << "edi: " << cpu_.edi << endl
			 << "eax: " << cpu_.eax << endl
			 << "EFLAGS: " << cpu_.EFLAGS << endl;

		cout << endl
			 << "////////////////////////// CALL STACK /////////////////////////////"<< endl;

			 for (int i = cpu_.call_stack.size() - 1; i >= 0; i--){
			 	if (i >= cpu_.rsp)
			 		cout << cpu_.call_stack[i] <<endl;
			 }
			 for (int i = cpu_.rsp - 1; i >= 0; i--){
			 	if (cpu_.accessed[i] == 1)
			 		cout << cpu_.call_stack[i] <<endl;
			 }

		cout<< endl;




		// [EMI] PRINT CACHE IN SIMILAR FORMAT TO ABOVE AND BELOW




		page_table();


		if(assembly_mem.at(cpu_.pc)[0] == 'j' || assembly_mem.at(cpu_.pc).substr(0,4) == "call")
			cpu_.execute_step(assembly_mem[cpu_.pc]);

		else if (cpu_.pc < instruction_size - 1) {
			cpu_.pc ++;
		}

		else cpu_.pc = 0;

		// FEATURE/BUG*** amount of individual characters entered equals how many steps function takes
		// 'a' takes 1 step
		// '123' takes 3 steps
		// 'helloworld' takes 10 steps
		cout << "press any key and 'Enter' to continue" << endl;
		cout << "FUNCTION WILL ONLY LOOP UNTIL CPU CLASS IS TIED IN TO RUN MACHINE CODE AND UPDATE REGISTERS" << endl;
		char x;
		cin >> x;

		for (int i = 0; i < 20; i++) {

			cout << endl;
		}

		step_through();
		
}