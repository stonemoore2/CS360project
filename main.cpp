#include "memory.cpp"
#include "encode.cpp"
#include "translator.cpp"
//#include "structs.cpp"

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

ifstream infile("input.txt");
string code = file_to_string(infile);

memory external = memory(2048);
vector<string> binary = string_to_binary(code);


/*int main(){
  cout<<"TRANSLATED SOURCE CODE"<<endl;
  string assembly = translate(code);
  cout<<assembly<<endl<<"MEMORY"<<endl;
  
	for (int j = 0; j < binary.size(); j++){
		external.setword(j, binary[j]);
	}
		cout << "| Word | Data |" << "\n";
	for (int i = 0; i < external.getsize(); i++){
	  printf ("|  %i   | %s |\n", i, external.getword(i).c_str());
    //code doesn't work on my mac without the .c_str() at the end. If this causes
    //a problem removing it should hopefully fix it - Emile
  }

}*/


int main(int argc, char** argv) 
{
  bool run_prog = true;
  std::string Source_In;
  std::string inputFile = "input.txt";
  
  memory external = memory(2048);
  vector<string> binary = string_to_binary(code);
  
  while (run_prog)
  {
    

    cout<<"Enter number for command:1. edit source code; 2. print converted assembly code; 3. memory; 4. exit" <<endl;
    
    string command;

    getline(std::cin, command);
    
    if(command == "1")
    {
      std::cout << "Please edit the input file " << inputFile << " directly, press any key to continue" << std::endl;
      std::cin.ignore(1);
      std::cin.clear();
      
      ifstream infile(inputFile.c_str());
      code = file_to_string(infile);
      infile.close();
    }
    else if(command=="2" )
  
    {
      /*if(Source_In != "")
      {
        //parse entire string, including tabs and the like, into MIPS assembly code.
        //Call function on Source_In string.
        //Store result in some container.
      }
      //call function that print converted assembly code
      //Print using cout or in-built print function.*/
      cout<<"TRANSLATED SOURCE CODE"<<endl;
      string assembly = translate(code);
      cout<<assembly<<endl;
    }
    
    else if(command == "3")
  
    {
        for (int j = 0; j < binary.size(); j++){
          external.setword(j, binary[j]);
        }
        cout << "| Word | Data |" << "\n";
  
        for (int i = 0; i < external.getsize(); i++)
        {
          
          printf ("|  %i   | %s |\n", i, external.getword(i).c_str()); 
        }
  
  

    }
    
    else if (command =="4")
  
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