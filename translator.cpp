#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

//Used to blocks of code within functions and if/for blocks
string block(string input) {
  int i = 0;
  string output;
  int scope = 0;
  
  while(i < input.length()) {
    if (input[i] == '{') {
      output.append(input,i,1);
      scope++;
    }
    else if ((input[i] == '}') && (scope <= 0)) {
      return output;
    }
    else if ((input[i] == '}') && (scope > 0)) {
      scope--;
      output.append(input,i,1);
    }
    else {
      output.append(input,i,1);
    }
    i++;
  }
  return output;
}

/*Keeps track of declared variables
  vector<string> variables;
  //keeps track of number of variables per function for subtracting from rbp
  vector<int> num_variables;
  //keeps track of declared functions
  vector<string> functions;
  //keeps track and helps make labels
  int labels = 1;
*/

void translate(string source, stringstream& output, vector<string>& variables, vector<int>& num_variables, vector<string>& functions, int& labels) {
  vector<string> components(6);
  int i = 0;
   while(i < source.length()) {

     //Ignores new lines and tabs
     while (source[i] == '\n' || source[i] == '\t')
       i++;

     //Helps determine what instruction is being used (Pt. 1)
     int pos[] = {0,0,0,0,0,0};
     pos[0] = source.find("(",i);
     pos[1] = source.find("[",i);
     pos[2] = source.find("=",i);
     pos[3] = source.find(" ",i);
     pos[4] = source.find("+",i);
     pos[5] = source.find("-",i);
     //conditionals not accounted for. Hard coded in for loops and if statements

     //Used to account for -1 return value if not present in code
    for (int j = 0; j < 6; j++) {
      if (pos[j] < 0)
	pos[j] = 1000000;
    }

    //End of Pt. 1
    int next = *(min_element(pos,pos+6));

    //if no recognizable functions, leave the program
    if (next > source.length())
      return;

    //gets the first part of source code instruction
    components[0] = source.substr(i,next-i);
    i = next;

    //will be declaring a variable or function
    if (components[0] == "int") {
      i++;

      //determines whether function, array, or variable declaration.
      pos[0] = source.find("(",i);
      pos[1] = source.find("[",i);
      pos[2] = source.find("=",i);
      for (int j = 0; j < 3; j++) {
	if (pos[j] < 0)
	pos[j] = 1000000;
    }
      int min = *(min_element(pos,pos+3));

      //Name of variable/function that is being declared
      components[1] = source.substr(i,min-i);

      if (source[min] == '(') {
	//A function is being declared
	
	i = min+1;

	next = source.find("[",i);
	min = source.find(")",i);

	string var;

	//Determines what the input type of the function is
	if ((next < min) && (next > -1)) {
	  components[2] = "(int*)";

	  int temp_pos = source.find(" ",i)+1;
	  var = source.substr(temp_pos,next-temp_pos)+"[0]";
	  variables.push_back(var);
	}
	else if (source.find(")",i) == i){
	  components[2] = "()";
	}
	else {
	  components[2] = "(int)";

	  
	  int temp_pos = source.find(" ",i)+1;
	  var = source.substr(temp_pos,min-temp_pos);
	  variables.push_back(var);
	}

	//Creates the function name to add to the list of functions
	string fn = components[1] + components[2];
	functions.push_back(fn);
	
	i = min+1;

	output<< fn << ":" << endl;
	output<< "push rbp"<<endl;
	output<< "mov rbp, rsp"<<endl;
	//A placeholder output that will be edited later
	output<< "POTENTIAL FUNCTION CALL"<<endl;

	if (components[2] == "(int*)") {

	  //Block of code that determines if it is a valid variable
	  
	  int sp = 0;
	  for (int j = 0;j < variables.size();j++) {
	  if (var == variables[j])
	    sp = j+1;
	  }
	
	if (sp <= 0) {
	  output << "ERROR" << endl;
	  return;
	}
	
	else {
	  string store = "[rbp-" + to_string(4*sp) + "]";
	  
	  output<< "mov " << store << ", " << "rdi"<<endl;
	}
	}
	
	else if (components[2] == "(int)") {

	  //Block of code that determines if it is a valid variable
	  
	  int sp = 0;
	for (int j = 0;j < variables.size();j++) {
	  if (var == variables[j])
	    sp = j+1;
	}

	if (sp <= 0) {
	  output << "ERROR" << endl;
	  return;
	}
	
	else {
	  string store = "[rbp-" + to_string(4*sp) + "]";
	  
	  output<< "mov " << store << ", " << "edi"<<endl;
	}
	}
      
	next = source.find("{",i);
	i=next+1;

	//translate is run recursively on the block of code in the function
	string jump = block(source.substr(i,source.length()-i));
	i=i+jump.length()+1;
	translate(jump,output,variables,num_variables,functions,labels);

	if (components[1] == "main")
	  output<<"mov eax, 0"<<endl;
	//A placeholder output that will be edited later
	output<<"POTENTIAL EXIT CALL"<<endl;
	output<<"ret"<<endl;

	//After function is translated clears the local variables for that function and adds them to num_variables for editing afterwards.
	num_variables.push_back(variables.size()*4);
	variables.clear();
      }

      else if (source[min] == '[') {
	//An Array is being declared

	i=min+1;
	min = source.find("]",i);
	components[2] = source.substr(i,min-i);

	int length = stoi(components[2]);

	//adds each element of the array to variables so that enough space is allocated
	for (int j = 0;j<length;j++)
	  {
	    variables.push_back(components[1] + "[" + to_string((length-j)-1) + "]");
	  }

	//this was supposed to be code that did different things if no base values for the array were specified
	//However I forgot about this so just make sure we always initialize at lease one element of the array
	
	i = min+1;
	min = source.find("{",i);
	next = source.find(";",i);
	
	if (min < next) {
	  i = min+1;
	  next = source.find("}",i);

	  //gets each initialized value of the array
	  vector<string> inputs;     
	  while ((source.find(",",i) > 0) && (source.find(",",i) < next)) {
	    min = source.find(",",i);
	    components[3] = source.substr(i,min-i);
	    inputs.push_back(components[3]);
	    i = min+1;
	  }
	  min = source.find("}",i);
	  components[3] = source.substr(i,min-i);
	  inputs.push_back(components[3]);
	  i = min+2;

	  
	  for (int j=0;j<inputs.size();j++) {
	    string store = "[rbp-" + to_string(4*(variables.size()-j)) + "]";
	    
	    string val;
	    int sp=0;

	    //determines whether initial values are other variables or numeric values
      for (int k = 0;k < variables.size();k++) {
	  if (inputs[j] == variables[k])
	    sp = k+1;
	  }


      //initial value is a variable
	  if (sp > 0) {
	     val = "[rbp-" + to_string(4*sp) + "]";
	     output<<"mov eax, "<<val<<endl;
	     output<<"mov "<<store<<", eax"<<endl;
	  }
	  //initial value is a number 
	  else {
	    val = to_string(stoi(inputs[j]));
	    output<<"mov "<<store<<", "<<val<<endl;
	  }

	  //Theoretically something could be added to make an error check but that's not neccesary.

	  }

	  for (int j=inputs.size();j<length;j++) {
	    string store = "[rbp-" + to_string(4*(variables.size()-j)) + "]";
	    output<<"mov "<<store<<", 0"<<endl;
	  }
	    
	}
      }

      else if (source[min] == '=') {
	//A variable is being declared
	variables.push_back(components[1]);
	i = min+1;
	min = source.find(";",i);
	components[2] = source.substr(i,min-i);
	i = min+1;

	string store = "[rbp-" + to_string((variables.size())*4) + "]";

	//a series of check that determine whether the stored value is more complex than a number or variable
	//all of them recursively call translate
	
	//math_check
	  if (((components[2].find("+",0) > 0) && (components[2].find("+",0) < components[2].length())) || ((components[2].find("-",0) > 0) && (components[2].find("-",0) < components[2].length()))) {
	    translate(components[2] + ";",output,variables,num_variables,functions,labels);
	    output<<"mov "<<store<<", " <<"eax"<<endl;
	}
	  //index_check
	  else if ((components[2].find("[",0) > 0) && (components[2].find("[",0) < components[2].length())) {
	    translate(components[2],output,variables,num_variables,functions,labels);
	  output<<"mov "<<store<<", " <<"eax"<<endl;
	}
	  //function_check
	  else if ((components[2].find("(",0) > 0) && (components[2].find("(",0) < components[2].length())) {
	    translate(components[2],output,variables,num_variables,functions,labels);
	  output<<"mov "<<store<<", " <<"eax"<<endl;
	}
	  //a non complex stored value
	  else {
	int sp=0;
	string val;

	for (int j = 0;j < variables.size();j++) {
	  if (components[2] == variables[j])
	    sp = j+1;
	}

	if (sp > 0) {
	  val = "[rbp-" + to_string(4*sp) + "]";
	}
	else {
	  val = to_string(stoi(components[2]));
	}
	
	sp = variables.size();
	store = "[rbp-" + to_string(4*sp) + "]";

	output<< "mov eax, "<<val<<endl;
	output<<"mov "<< store <<", eax" <<endl;
	  }
      }
    }
    
    else if (components[0] == "for") {
      //translates for loops
      //comp[1] = variable declaration, comp[3] = conditional, comp[2] = arithmetic
      i++;
      next = source.find(";",i);
      components[1] = source.substr(i,next-i);

      //runs the variable declaration
      translate(components[1] + ";",output,variables,num_variables,functions,labels);
      string loop_label = ".L" + to_string(labels);
      labels++;
      output<<loop_label+":"<<endl;      
      i=next+1;

      next = source.find(";",i);
      components[3] = source.substr(i,next-i);
      i=next+1;

      next = source.find(")",i);
      components[2] = source.substr(i,next-i);
      i=next+1;
      next = source.find("{",i);
      i=next+1;

      string jump = block(source.substr(i,source.length()-i));
      i=i+jump.length()+1;

      string conditional = components[3];
      int opcode = 0;

      //these are the values that are being compared for the conditional
      components[4] = "";
      components[5] = "";

      //determines what comparison is being used
      if ((conditional.find("<=",0) > 0) && (conditional.find("<=",0) < conditional.length())) {
	opcode = 1;
	
	int j = conditional.find("<=",0);
	components[4] = conditional.substr(0,j);
	components[5] = conditional.substr(j+2,conditional.length()-(j+2));
      }
      else if (conditional.find(">=",0) > 0  && (conditional.find(">=",0) < conditional.length())) {
	opcode = 2;

	int j = conditional.find(">=",0);
	components[4] = conditional.substr(0,j);
	components[5] = conditional.substr(j+2,conditional.length()-(j+2));
      }
	else if (conditional.find("!=",0) > 0  && (conditional.find("!=",0) < conditional.length())) {
	opcode = 3;

	int j = conditional.find("!=",0);
	components[4] = conditional.substr(0,j);
	components[5] = conditional.substr(j+2,conditional.length()-(j+2));
      }
	else if (conditional.find("==",0) > 0 && (conditional.find("==",0) < conditional.length())) {
	opcode = 4;

	int j = conditional.find("==",0);
	components[4] = conditional.substr(0,j);
	components[5] = conditional.substr(j+2,conditional.length()-(j+2));
      }
	else if (conditional.find(">",0) > 0 && (conditional.find(">",0) < conditional.length())) {
	opcode = 5;

	int j = conditional.find(">",0);
	components[4] = conditional.substr(0,j);
	components[5] = conditional.substr(j+1,conditional.length()-(j+1));
      }
	else if (conditional.find("<",0) > 0 && (conditional.find("<",0) < conditional.length())) {
	opcode = 6;

	int j = conditional.find("<",0);
	components[4] = conditional.substr(0,j);
	components[5] = conditional.substr(j+1,conditional.length()-(j+1));
      }
      else {
	output<<"ERROR"<<endl;
	return;
      }

      //first value being used in cmp statement
      string op1;
 	  
      int sp = 0;
      for (int j = 0;j < variables.size();j++) {
	if (components[4] == variables[j])
	  sp = j+1;
      }

      if (sp > 0) {
	op1 = "[rbp-" + to_string(4*sp) + "]";
      }
      else {
	op1 = to_string(stoi(components[4]));
      }

      //second value being used in cmp statement
      string op2;
      
      sp = 0;
      for (int j = 0;j < variables.size();j++) {
	if (components[5] == variables[j])
	  sp = j+1;
      }

      if (sp > 0) {
	op2 = "[rbp-" + to_string(4*sp) + "]";
      }
      else {
	op2 = to_string(stoi(components[5]));
      }

      output<<"cmp "<<op1<<", "<<op2<<endl;

      string exit_label = ".L" + to_string(labels);
      labels++;

      //determines the jump function
      if (opcode == 1)
	output<<"jg "<<exit_label<<endl;
      else if (opcode == 2)
	output<<"jl "<<exit_label<<endl;
      else if (opcode == 3)
	output<<"je "<<exit_label<<endl;
      else if (opcode == 4)
	output<<"jne "<<exit_label<<endl;
      else if (opcode == 5)
	output<<"jle "<<exit_label<<endl;
      else if (opcode == 6)
	output<<"jge "<<exit_label<<endl;

      //recursively calls translate on block of code
       translate(jump,output,variables,num_variables,functions,labels);
      //recursively calls translate on loop iterator
        translate(components[2] + ";",output,variables,num_variables,functions,labels);
      output<<"jmp "<<loop_label<<endl;
      output<<exit_label+":"<<endl;
      
    }
    else if (components[0] == "if") {
      //translates if
      //comp[1] = conditional
      i++;
      next = source.find(")",i);
      components[1] = source.substr(i,next-i);
      i=next+1;
      next = source.find("{",i);
      i=next+1;

      string jump = block(source.substr(i,source.length()-i));
      i=i+jump.length()+1;
      
      string conditional = components[1];
      int opcode = 0;
      

      //these are the values that are being compared for the conditional
      components[2] = "";
      components[3] = "";

      //determines what comparison is being used
      if ((conditional.find("<=",0) > 0) && (conditional.find("<=",0) < conditional.length())) {
	opcode = 1;
	
	int j = conditional.find("<=",0);
	components[2] = conditional.substr(0,j);
	components[3] = conditional.substr(j+2,conditional.length()-(j+2));
      }
      else if (conditional.find(">=",0) > 0  && (conditional.find(">=",0) < conditional.length())) {
	opcode = 2;

	int j = conditional.find(">=",0);
	components[2] = conditional.substr(0,j);
	components[3] = conditional.substr(j+2,conditional.length()-(j+2));
      }
	else if (conditional.find("!=",0) > 0  && (conditional.find("!=",0) < conditional.length())) {
	opcode = 3;

	int j = conditional.find("!=",0);
	components[2] = conditional.substr(0,j);
	components[3] = conditional.substr(j+2,conditional.length()-(j+2));
      }
	else if (conditional.find("==",0) > 0 && (conditional.find("==",0) < conditional.length())) {
	opcode = 4;

	int j = conditional.find("==",0);
	components[2] = conditional.substr(0,j);
	components[3] = conditional.substr(j+2,conditional.length()-(j+2));
      }
	else if (conditional.find(">",0) > 0 && (conditional.find(">",0) < conditional.length())) {
	opcode = 5;

	int j = conditional.find(">",0);
	components[2] = conditional.substr(0,j);
	components[3] = conditional.substr(j+1,conditional.length()-(j+1));
      }
	else if (conditional.find("<",0) > 0 && (conditional.find("<",0) < conditional.length())) {
	opcode = 6;

	int j = conditional.find("<",0);
	components[2] = conditional.substr(0,j);
	components[3] = conditional.substr(j+1,conditional.length()-(j+1));
      }
      else {
	output<<"ERROR"<<endl;
	return;
      }

      int sp;

      //first value for cmp statement
      string op1;

      //determines if a complex value is being compated
      //math_check
      if (((components[2].find("+",0) > 0) && (components[2].find("+",0) < components[2].length())) || ((components[2].find("-",0) > 0) && (components[2].find("-",0) < components[2].length()))) {
	 translate(components[2] + ";",output,variables,num_variables,functions,labels);
	    op1 = "eax";
	}
	  //index_check
      else if ((components[2].find("[",0) > 0) && (components[2].find("[",0) < components[2].length())) {
	 translate(components[2],output,variables,num_variables,functions,labels);
	    op1 = "eax";
	}
      //a number or variable
      else {

	//determines whether a number or variable
      sp = 0;
      for (int j = 0;j < variables.size();j++) {
	if (components[2] == variables[j])
	  sp = j+1;
      }

      if (sp > 0) {
	op1 = "[rbp-" + to_string(4*sp) + "]";
      }
      else {
	op1 = to_string(stoi(components[2]));
      }
      }

      //near identical to 1st half
      string op2;

      //math_check
      if (((components[3].find("+",0) > 0) && (components[3].find("+",0) < components[3].length())) || ((components[3].find("-",0) > 0) && (components[3].find("-",0) < components[3].length()))) {
	 translate(components[3] + ";",output,variables,num_variables,functions,labels);
	    op2 = "eax";
	}
	  //index_check
      else if ((components[3].find("[",0) > 0) && (components[3].find("[",0) < components[3].length())) {
	 translate(components[3],output,variables,num_variables,functions,labels);
	    op2 = "eax";
	}
      
      else {
	
      sp = 0;
      for (int j = 0;j < variables.size();j++) {
	if (components[3] == variables[j])
	  sp = j+1;
      }

      if (sp > 0) {
	op2 = "[rbp-" + to_string(4*sp) + "]";
      }
      else {
	op2 = to_string(stoi(components[3]));
      }
      }

      output<<"cmp "<<op1<<", "<<op2<<endl;

      string label = ".L" + to_string(labels);
      labels++;

      //determines jump statement
      if (opcode == 1)
	output<<"jg "<<label<<endl;
      else if (opcode == 2)
	output<<"jl "<<label<<endl;
      else if (opcode == 3)
	output<<"je "<<label<<endl;
      else if (opcode == 4)
	output<<"jne "<<label<<endl;
      else if (opcode == 5)
	output<<"jle "<<label<<endl;
      else if (opcode == 6)
	output<<"jge "<<label<<endl;

      //recursively calls translate on block of code
      translate(jump,output,variables,num_variables,functions,labels);
      output<<label+":"<<endl;
		
	  
    }
    else if (components[0] == "return") {
      //a return statement is being called
      //comp[1] = value being returned
      i++;
      next = source.find(";",i);
      components[1] = source.substr(i,next-i);

      //determines whether a number or variable is being returned
      string val;
      int sp=0;

      for (int j = 0;j < variables.size();j++) {
	  if (components[1] == variables[j])
	    sp = j+1;
	  }

	  if (sp > 0) {
	     val = "[rbp-" + to_string(4*sp) + "]";
	  }
	  else {
	    val = to_string(stoi(components[1]));
	  }
      
      output<<"mov eax, "<<val<<endl;
      i = next+1;

      //lacking check for complex return values but shouldn't be too hard to add
    }

    //The first part of the source code instruction is a variable or a function
    else {
      if (source[next] == '(') {
	//A function is being called	

	string call = "";

	//checks if function being called exists
	for (int j = 0;j < functions.size();j++) {
	  string fn = functions[j];
	  int min = fn.find("(",0);
	  fn = fn.substr(0,min);
	    
	  if (components[0] == fn)
	    call = functions[j];
	}

	//should prob add an error message if an invalid function call but...

	//checks what input type the function has
	next = call.find("(",0);
	string input = call.substr(next,call.length()-1);

	//input is a single variable
	if (input == "(int)") {
	  i++;
	
	  next = source.find(")",i);
	  components[1] = source.substr(i,next-i);

	  //checks if a valid input call
	  if (components[1] == "") {
	    output<<"ERROR"<<endl;
	    return;
	  }

	  //checks if input is variable or number
	  string val;	  
	  int sp = 0;
	  for (int j = 0;j < variables.size();j++) {
	  if (components[1] == variables[j])
	    sp = j+1;
	  }

	  if (sp > 0) {
	     val = "[rbp-" + to_string(4*sp) + "]";
	  }
	  else {
	    val = to_string(stoi(components[1]));
	  }
	  
	  output<<"mov eax, "<<val<<endl;
	  output<<"mov edi, eax"<<endl;
	  output<<"call "<<call<<endl;
	  i = next+2;
	}
	//input is an array
	else if (input == "(int*)") {
	  i++;	
	  next = source.find(")",i);
	  components[1] = source.substr(i,next-i);

	  //checks if a valid input
	  if (components[1] == "") {
	    output<<"ERROR"<<endl;
	    return;
	  }

	  //adjustement for how arrays are stored in variables array
	  components[1] = components[1] + "[0]";

	  //checks for valid array input
	  string val;
	  int sp = 0;
	  for (int j = 0;j < variables.size();j++) {
	  if (components[1] == variables[j])
	    sp = j+1;
	  }

	  if (sp > 0) {
	     val = "[rbp-" + to_string(4*sp) + "]";
	  }
	  else {
	    output<<"ERROR"<<endl;
	    return;
	  }
	  
	  output<<"mov rax, "<<val<<endl;
	  output<<"mov rdi, rax"<<endl;
	  output<<"call "<<call<<endl;
	  i = next+2;
	}
	//function call with no input
	else {
	  i++;	
	  next = source.find(")",i);	  
	  output<<"call "<<call<<endl;	  
	  i=next+2;
	}
	
      }
      else if (source[next] == '[') {
	//An array is being indexed

	i++;
	next = source.find("]",i);
	components[1] = source.substr(i,next-i);
	i = next+1;

	//checks if array being accessed has been declared previously
	string var = components[0] + "[0]";
	int sp = 0;
	for (int j = 0;j < variables.size();j++) {
	  if (var == variables[j])
	    sp = j+1;
	}
	
	if (sp <= 0) {
	  output << "ERROR" << endl;
	  return;
	}
	
	else {
	string store = "[rbp-" + to_string(4*sp) + "]";
	string val;	  
	sp = 0;

	//determines whether index is a variable or number
	for (int j = 0;j < variables.size();j++) {
	  if (components[1] == variables[j])
	    sp = j+1;
	}

	//used for variable index
	if (sp > 0) {
	  val = "[rbp-" + to_string(4*sp) + "]";

	  output<<"mov eax, "<<val<<endl;
	  output<<"cdqe"<<endl;
	  output<<"lea rdx, [0+rax*4]"<<endl;
	  output<<"mov rax, "<<store<<endl;
	  output<<"add rax, rdx"<<endl;
	  output<<"mov eax, [rax]"<<endl;
 
	}
	//used for numeric index
	else {
	 val = to_string(stoi(components[1]));
	 output<<"mov rax, "<<store<<endl;
	 store = "[rax+" + to_string((stoi(val)*4)) + "]";	 
	 output<<"mov eax, "<<store<<endl;
	}
	}
      }

      else if (source[next] == '+' || source[next] == '-') {
	//Assumes format is X + Y;
	//Arithmetic is being used
	//Multiplication,Division, and Modulo is not implemented
	i++;

	//determines what arithmetic operation is being used
	int op = 0;
	if (source[next] == '+')
	  op = 1;
	else if (source[next] == '-')
	  op = 2;

	next = source.find(";",i);
	components[1] = source.substr(i,next-i);
	//comp[1] is X in above formula

	//checks if X is a declared variable, since the new value is stored to it
	int sp = 0;
	for (int j = 0;j < variables.size();j++) {
	  if (components[0] == variables[j])
	    sp = j+1;
	}
	
	if (sp <= 0) {
	  output << "ERROR" << endl;
	  return;
	}

	else {
	  //gets the pointer for X
	  string store = "[rbp-" + to_string(4*sp) + "]";

	  //determines whether Y is a variable or number
	  string val;	  
	  sp = 0;
	  for (int j = 0;j < variables.size();j++) {
	  if (components[1] == variables[j])
	    sp = j+1;
	  }

	  if (sp > 0) {
	     val = "[rbp-" + to_string(4*sp) + "]";
	  }
	  else {
	    val = to_string(stoi(components[1]));
	  }

	  output<<"mov eax, "<<store<<endl;
	  //determines the arithmetic operation used
	  if (op == 1)
	    output<<"add "<<"eax"<<", " <<val<<endl;
	  else if (op == 2)
	    output<<"sub "<<"eax"<<", " <<val<<endl;
	i = next+1;
	}
      }

      //this could be an else if, but it's not...
      else {
	//A value is being assigned to a variable	
      if (source[next] == '=') {
	i++;
	next = source.find(";",i);
	components[1] = source.substr(i,next-i);
	//components[1] is the variable value is being stored to

	//determines if store value is valid
	int sp = 0;
	for (int j = 0;j < variables.size();j++) {
	  if (components[0] == variables[j])
	    sp = j+1;
	}
	
	if (sp <= 0) {
	  output << "ERROR" << endl;
	  return;
	}
	
	else {
	  //gets pointer for store variable
	  string store = "[rbp-" + to_string(4*sp) + "]";	  
	  string val;

	  //checks if something complex is being assigned
	  //math_check
	  if (((components[1].find("+",0) > 0) && (components[1].find("+",0) < components[1].length())) || ((components[1].find("-",0) > 0) && (components[1].find("-",0) < components[1].length()))) {
	     translate(components[1] + ";",output,variables,num_variables,functions,labels);
	  output<<"mov "<<store<<", " <<"eax"<<endl;
	}
	  //index_check
	  else if ((components[1].find("[",0) > 0) && (components[1].find("[",0) < components[1].length())) {
	     translate(components[1],output,variables,num_variables,functions,labels);
	  output<<"mov "<<store<<", " <<"eax"<<endl;
	}
	  //function_check
	  else if ((components[1].find("(",0) > 0) && (components[1].find("(",0) < components[1].length())) {
	     translate(components[1],output,variables,num_variables,functions,labels);
	  output<<"mov "<<store<<", " <<"eax"<<endl;
	}
	  //value being stored is one variable or a number
	  else {
	  
	  sp = 0;
	  for (int j = 0;j < variables.size();j++) {
	  if (components[1] == variables[j])
	    sp = j+1;
	  }

	  if (sp > 0) {
	     val = "[rbp-" + to_string(4*sp) + "]";
	  }
	  else {
	    val = to_string(stoi(components[1]));
	  }
	  
	output<<"mov eax, "<<val<<endl;
	output<<"mov "<<store<<", " <<"eax"<<endl;
	  }
	i = next+1;
	}
      }
      }
    }
   }
}

string translate(string source) {
  //Keeps track of declared variables
  vector<string> variables;
  //keeps track of number of variables per function for subtracting from rbp
  vector<int> num_variables;
  //keeps track of declared functions
  vector<string> functions;
  //keeps track and helps make labels
  int labels = 1;
  stringstream output;

  translate(source,output,variables,num_variables,functions,labels);
  string value = output.str();

  //this accounts for whether a not a function calls another function
  int i = 0;
  for (int count = 0; count < functions.size(); count++) {

    int call_check = value.find("call ",i);
    int ret_check = value.find("ret",i);
    
  if ((call_check < ret_check) && (call_check > -1)) {
    string sub = "sub rbp, " + to_string(num_variables[count]);
    
    i = value.find("POTENTIAL FUNCTION CALL",0); //23
    value.replace(i,23,sub);

    i = value.find("POTENTIAL EXIT CALL",0); //19
    value.replace(i,19,"leave");

    i = ret_check+1;
  }
  else {
    i = value.find("POTENTIAL FUNCTION CALL",0); //23
    value.replace(i,24,"");

    i = value.find("POTENTIAL EXIT CALL",0); //19
    value.replace(i,19,"pop rbp");

    i = ret_check+1;
  }
  }
  return value;
}

/*
int main(){
  ifstream t("file.txt");
  stringstream buffer;
  buffer << t.rdbuf();

  string source3 = buffer.str();
  
  string source = "int total(int b){int max=0;for(int i=1;3>i;i=i+1){int min=max;min=2;max=min;return max;int a[7]={16,2,3,4,5,6,7};if(2<b){total(min);}a[min];}max+min;}";
  string source2 = "int total(int a[5]){int max=0;for(int i=0;i<5;i=i+1){if(max<a[i]){max=a[i];}}return max;}int main(){int a[5]={16,2,77,40,5};int max=total(a);}";

  string assembly = translateWrap(source3);
  cout<<assembly<<endl;

      }
*/
