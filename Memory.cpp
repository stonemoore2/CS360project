#include <fstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

// Keeps track of how steps have been taken during step through
int num_of_iteration = 1;

// Keeps track of current address being pointed to by PC, PC initialized as -1 until address of main() is determined
int PC = -1;

// Keeps track of how many lines machine_code and assembly code needs
int instruction_size = 0;

// Stores assemby code as vector of strings for easy access
vector<string> assembly;

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
 	string machine_code;

 	while (ifsM.good()) {

 		getline(ifsM,machine_code);

 		machine.push_back(machine_code);
 	}

 	while (ifsA.good()) {

 		getline(ifsA,instr);

 		if (instr.substr(0,1) == "." || instr.substr(0,5) == "total"){

 			cout << instr << endl;
 			function_names_virt.push_back(instr);
 			function_index_virt.push_back(instruction_counter);

 		}

 		else if (instr.substr(0,4) == "main") {

 			PC = instruction_counter;
 			cout << instr << endl;

 			function_names_virt.push_back(instr);
 			function_index_virt.push_back(instruction_counter);
 		}

 		else {

 			if (PC == instruction_counter) {

 				string PC_string = to_string(instruction_counter);

 				string substring = "";

 				substring = substring + "PC==>(" + PC_string + ") " + instr;

 				string output = "";

 				//string mac = "";

 				//mac = machine[instruction_counter];

 				output = output + substring;

 				cout << output << endl;

	 			instruction_counter++;

	 			assembly.push_back(instr);
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

	 			assembly.push_back(instr);
	 		}
 		}
 	}

 	instruction_size = instruction_counter;
}

// WORK IN PROGRESS step_through() will run single lines of code starting from main and update register values as it does so
void step_through() {

	cout << "Step: " << num_of_iteration << endl << endl;

	num_of_iteration++;
/*
	for (int i = 0; i < function_names_virt.size(); i++) {

		cout << function_names_virt[i] << endl << function_index_virt[i] << endl;
	}
*/
		cout << "//////////////////////// VIRTUAL MEMORY ///////////////////////////" << endl;

		for (int i = 0; i < instruction_size - 1; i++) {

			if (PC == i) {

				cout << "PC==> " << "[" << i << "]\t" << machine[i].substr(0,32) << endl;
			}

			else cout << "      " << "[" << i << "]\t" << machine[i].substr(0,32) << endl;
		}

		cout << endl
			 << "///////// CURRENT INSTRUCTION & CURRENT MACHINE CODE///////////////"<< endl;

		cout << "       " << assembly[PC] << endl
			 << "       " << machine[PC]<< endl;

		cout << endl
			 << "///////////////////// REGISTER VALUES /////////////////////////////"<< endl;

		cout << "PC:  " << PC << endl
			 << "rbp: " << endl
			 << "rsp: " << endl
			 << "rdi: " << endl
			 << "rax: " << endl
			 << "rdx: " << endl
			 << "edi: " << endl
			 << "eax: " << endl;

		if (PC < instruction_size - 2) {

			PC++;
		}

		else PC = 0;

		// FEATURE/BUG*** amount of individual characters entered equals how many steps function takes
		// 'a' takes 1 step
		// '123' takes 3 steps
		// 'helloworld' takes 10 steps
		cout << "press any key and 'Enter' to continue" << endl;
		char x;
		cin >> x;

		for (int i = 0; i < 20; i++) {

			cout << endl;
		}

		step_through();
		
}