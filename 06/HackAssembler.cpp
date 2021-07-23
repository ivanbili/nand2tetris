#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <vector>

std::string intToBinaryStr(int val, int bits)
{
    std::string binStr;
    while (bits!=0)
       {
       binStr = (val%2==0 ?"0":"1")+binStr;
       val/=2;
       bits--;
       }
    return binStr;
}

class SymbolTable
   {
   private:
   std::unordered_map<std::string, unsigned int> _name_to_address;
   public:
   SymbolTable()
      {
      _name_to_address["R0"] = 0;
      _name_to_address["R1"] = 1;
      _name_to_address["R2"] = 2;
      _name_to_address["R3"] = 3;
      _name_to_address["R4"] = 4;
      _name_to_address["R5"] = 5;
      _name_to_address["R6"] = 6;
      _name_to_address["R7"] = 7;
      _name_to_address["R8"] = 8;
      _name_to_address["R9"] = 9;
      _name_to_address["R10"] = 10;
      _name_to_address["R11"] = 11;
      _name_to_address["R12"] = 12;
      _name_to_address["R13"] = 13;
      _name_to_address["R14"] = 14;
      _name_to_address["R15"] = 15;
      _name_to_address["SCREEN"] = 16384;
      _name_to_address["KBD"] = 24576;
      _name_to_address["SP"] = 0;
      _name_to_address["LCL"] = 1;
      _name_to_address["ARG"] = 2;
      _name_to_address["THIS"] = 3;
      _name_to_address["THAT"] = 4;
      }
   void assignValues()
      {
      unsigned int addr = 16;
      for(auto it=_name_to_address.begin();it!=_name_to_address.end();it++)
         {
         if (it->second == 0 && it->first != "SP" && it->first != "R0")
            it->second = addr++;
         }
      }
   unsigned int addName(std::string name)
      {
      return _name_to_address[name];
      }
   bool hasName(std::string name)
      {
      return _name_to_address.find(name) != _name_to_address.end();
      }
   void setAddressForName(std::string name, unsigned int address)
      {
      _name_to_address[name] = address;
      }
   unsigned int getAddressFromName(std::string name)
      {
      return _name_to_address[name];
      }
   };

class Code
   {
   private:
   std::unordered_map<std::string, std::string> _jmp_to_binary;
   std::unordered_map<std::string, std::string> _comp_to_binary;
   std::unordered_map<std::string, std::string> _dest_to_binary;
   public:
   void init()
      {
      _comp_to_binary["0"]   = "0101010";
      _comp_to_binary["1"]   = "0111111";
      _comp_to_binary["-1"]  = "0111010";
      _comp_to_binary["D"]   = "0001100";
      _comp_to_binary["A"]   = "0110000";
      _comp_to_binary["M"]   = "1110000";
      _comp_to_binary["!D"]  = "1110000";
      _comp_to_binary["!A"]  = "0110001";
      _comp_to_binary["!M"]  = "1110001";
      _comp_to_binary["-D"]  = "0001111";
      _comp_to_binary["-A"]  = "0110011";
      _comp_to_binary["-M"]  = "1110011";
      _comp_to_binary["D+1"] = "0011111";
      _comp_to_binary["A+1"] = "0110111";
      _comp_to_binary["M+1"] = "1110111";
      _comp_to_binary["D-1"] = "0001110";
      _comp_to_binary["A-1"] = "0110010";
      _comp_to_binary["M-1"] = "1110010";
      _comp_to_binary["D+A"] = "0000010";
      _comp_to_binary["D+M"] = "1000010";
      _comp_to_binary["D-A"] = "0010011";
      _comp_to_binary["D-M"] = "1010011";
      _comp_to_binary["A-D"] = "0000111";
      _comp_to_binary["M-D"] = "1000111";
      _comp_to_binary["D&A"] = "0000000";
      _comp_to_binary["D&M"] = "1000000";
      _comp_to_binary["D|A"] = "0010101";
      _comp_to_binary["D|M"] = "1010101";
      _dest_to_binary["null"]  = "000";
      _dest_to_binary["M"]   = "001"; 
      _dest_to_binary["D"]   = "010"; 
      _dest_to_binary["MD"]  = "011";
      _dest_to_binary["A"]   = "100"; 
      _dest_to_binary["AM"]  = "101";
      _dest_to_binary["AD"]  = "110";
      _dest_to_binary["AMD"] = "111";
      _jmp_to_binary["null"] = "000"; 
      _jmp_to_binary["JGT"]  = "001";
      _jmp_to_binary["JEQ"]  = "010";
      _jmp_to_binary["JGE"]  = "011";
      _jmp_to_binary["JLT"]  = "100";
      _jmp_to_binary["JNE"]  = "101";
      _jmp_to_binary["JLE"]  = "110";
      _jmp_to_binary["JMP"]  = "111";
      }
   std::string getBinaryFromComp(std::string val)
      {
      return _comp_to_binary[val];
      }	
   std::string getBinaryFromDest(std::string val)
      {
      return _dest_to_binary[val];
      }	
   std::string getBinaryFromJmp(std::string val)
      {
      return _jmp_to_binary[val];
      }	
   };

class Command
   {
   public:
   virtual std::string getBinary(SymbolTable st, Code codegen) = 0;
   };

class ACommandVariable : public Command
   {
   std::string _variable;
   public:
   ACommandVariable(std::string var)
      {
   	  _variable = var;
      }
   std::string getBinary(SymbolTable st, Code codegen)
      {
      return "0"+intToBinaryStr(st.getAddressFromName(_variable), 15);
      }
   };

class ACommandImmediate : public  Command
   {
   unsigned int _address_value;
   public:
   ACommandImmediate(unsigned int val)
   {
   _address_value = val;
   }
   std::string getBinary(SymbolTable st, Code codegen)
      {
      return "0"+intToBinaryStr(_address_value, 15);
      }
   };

class CCommand : public Command
   {
   std::string _dest;
   std::string _comp;
   std::string _jmp;
   public:
   CCommand(std::string dest, std::string comp, std::string jmp)
      {
      _dest = dest;
      _comp = comp;
      _jmp = jmp;
      }
   std::string getBinary(SymbolTable st, Code codegen)
      {
      return "111" + codegen.getBinaryFromComp(_comp) + codegen.getBinaryFromDest(_dest) + codegen.getBinaryFromJmp(_jmp);
      }
   };

class Parser
   {
   public:
   Command * createCommand(std::string & line, SymbolTable & sym_table, unsigned int counter)
      {
      std::string stripped;
      bool comment = false;
      for(std::string::size_type i = 0; i < line.size(); ++i)
      	 {
      	 if (line[i] == ' ' || line[i] == '\r' || line[i] == '\n')
      	    continue;
      	 int j = line[i];
      	 stripped.push_back(line[i]);
         }
      for(std::string::size_type i = 0; i < stripped.size(); ++i)
      	 {
      	 if (stripped[i] == '/' && (i + 1 < stripped.size()) && stripped[i] == '/')
      	 	  {
      	 	  stripped.resize(i);
      	    break;
      	    }
         }
      if (stripped.size() == 0)
         return NULL;
      if (stripped[0] == '@')
         {
         stripped = stripped.substr(1,stripped.size());
         if (stripped[0] >= '0' && stripped[0] <= '9')	
         	  {
            return new ACommandImmediate((unsigned int) std::stoi(stripped));
            }
         else if (sym_table.hasName(stripped))
         	  {
         	  if (sym_table.getAddressFromName(stripped) == 0)
         	     return new ACommandVariable(stripped);
         	  else
         	  	 return new ACommandImmediate(sym_table.getAddressFromName(stripped));
         	  }
         else
         	  {
         	  sym_table.addName(stripped);
         	  return new ACommandVariable(stripped);
         	  }
         }
      std::size_t found_equals = stripped.find('=');
      std::size_t found_amp = stripped.find(';');
      if (found_equals != std::string::npos)
         return new CCommand(stripped.substr(0,found_equals), stripped.substr(found_equals+1,stripped.size()-found_equals+1), "null");
      else if (found_amp != std::string::npos)
      	 return new CCommand("null", stripped.substr(0,found_amp), stripped.substr(found_amp+1,stripped.size()-found_amp+1));
      if (stripped[0] == '(')
      	 sym_table.setAddressForName(stripped.substr(1,stripped.size()-2), counter);
      return NULL;
      }
   };

int main (int argc, char* argv[])
   {
   std::ifstream inputFile(argv[1]);
   Parser p;
   Code c;
   c.init();
   SymbolTable symTable;
   std::vector<Command*> instructions;
   std::vector<std::string> source;
   unsigned int counter = 0;
   for(std::string line; getline( inputFile, line );)
   	   {
   	   Command * comm = p.createCommand(line, symTable, counter);
   	   if (comm)
   	   	  {
   	   	  counter++;
   	      instructions.push_back(comm);
   	      source.push_back(line);
   	      }
       }
   inputFile.close();
   symTable.assignValues();
   std::ofstream outFile("out.hack");
   for (int i = 0; i < instructions.size(); i++)
      outFile << instructions[i]->getBinary(symTable, c) << std::endl;
   outFile.close();
	 return 0;         
   }
