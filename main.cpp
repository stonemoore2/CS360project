#include "encode.cpp"
#include "translator.cpp"
#include "conversion.cpp"
#include "cpu.cpp"
#include "Memory.cpp"


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>

string file_to_string(ifstream& in){
    stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

int main(int argc, char** argv) 
{
  ifstream infile("input.txt");
  string code = file_to_string(infile);
  string assembly;
  infile.close();

  vector<string> machine_code;
  vector<string> binary = string_to_binary(code);

  bool run_prog = true;
  
  while (run_prog)
  {
    

    cout<<"Enter number for command:\n1. edit source code\n2. print converted assembly code\n3. full memory\n4. memory range\n";
    cout<<"5. assembly to machine code\n6. write assembly and machine code to files\n7. step_through_program\n8. exit" <<endl;
    
    string command;

    getline(std::cin, command);
    
    if(command == "1")
    {
      std::cout << "Please edit the input file input.txt directly (omit unnecessary spaces), press any key to continue" << std::endl;
      std::cin.ignore(1);
      std::cin.clear();
      
      ifstream infile("input.txt");
      code = file_to_string(infile);
      infile.close();
      binary = string_to_binary(code);
    }
    else if(command=="2" )
  
    {
      cout<<"TRANSLATED SOURCE CODE"<<endl;
      assembly = translate(code);
      cout<<assembly<<endl;
    }
    
    else if(command == "3")
  
    {

        cout << "| Word | Data |" << "\n";
        for (int i = 0; i < binary.size(); i++)
        {
          printf ("|  %i   | %s |\n", i, binary[i].c_str());
        }
  
    }
    

    else if (command =="4"){
      int beg, end;
      cout<< "Enter beginning of memory range: ";
      cin>> beg;
      cout<< "Enter end of memory range: ";
      cin>> end;
      if (beg < end && beg >= 0 && end <= 2047){
        cout << "| Word | Data |" << "\n";
        for (int i = beg; i < end + 1; i++){
          printf ("|  %i   | %s |\n", i, binary[i].c_str());
        }
      }
      else
        cout<< "invalid range" << "\n";
    }

    else if (command =="5")
  
    {
  
      assembly = translate (code);
      machine_code = assembly_to_machine(assembly);
      for (string s: machine_code){
        cout << s << '\n';
      }
    }

    else if (command =="6")
  
    {
      assembly = translate (code);
      machine_code = assembly_to_machine(assembly);
      ofstream filewrite;
      istringstream iss(assembly);
      filewrite.open("assembly.txt");
      for (string line; getline(iss, line);)
        filewrite << line << "\n";
      filewrite.close();

      filewrite.open("machine_code.txt");
      for (string x:machine_code)
        filewrite << x << "\n";
      filewrite.close();
      cout<< "assembly.txt and machine_code.txt written\n";
    }
    
    else if (command =="7")
  
    {
  
      virtual_memory();

	  step_through();
      
    }

    else if (command =="8")
  
    {
  
      run_prog = false;
      
    }

    else

    {

      cout<<"Please enter a valid command"<<endl;
      
    }
  
  }
  
  return 0;
}