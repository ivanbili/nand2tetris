#include <fstream>
#include <iostream>
#include <assert.h>
#include <cstring>

enum CommandType {C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL, C_NOP, C_UNKNOWN};

class Parser
   {
   private:
   std::ifstream _inputFile;
   std::string _line;
   std::string _arg1;
   int _arg2;
   CommandType _type;
   public:
   Parser(std::string inputFileName): _inputFile(inputFileName) {}
   ~Parser() {_inputFile.close();}
   bool hasMoreCommands() {return _line.size() != 0;}
   std::string arg1() {return _arg1;}
   int arg2() {return _arg2;}
   std::string getLine() {return _line;}
   CommandType commandType() {return _type;}
   void advance()
      {
      #define MAX_TOKEN_SIZE 16
      _type = C_UNKNOWN;
      getline(_inputFile, _line);
      const char * chars = _line.data();
      char currToken[MAX_TOKEN_SIZE] = "";
      int currTokenSize = 0;
      bool firstSlashSeen = false;
      
      _arg1.clear();
      bool arg2Set = false;
      
      for (int i = 0; i < strlen(chars); i++)
         {
         char currChar = chars[i];
         if (currChar == '/' && firstSlashSeen ) // the rest of the line is a comment
            {
            _type = C_NOP;
            break;
            }
         else if (currChar == ' ' || currChar == '/' || currChar == '\n' || currChar == '\r') // end of token
            {
            if (currChar == '/')
               firstSlashSeen = true;
            if (currTokenSize > 0)
               {
               if (_type == C_UNKNOWN)
                  {
                  // figure out type
                  if (strcmp(currToken, "push") == 0)
                     {
                     _type = C_PUSH;
                     }
                  else if (strcmp(currToken, "pop") == 0)
                     {
                     _type = C_POP; 
                     }
                  else if (strcmp(currToken, "add") == 0 ||
                           strcmp(currToken, "sub") == 0 ||
                           strcmp(currToken, "neg") == 0 ||
                           strcmp(currToken, "eq") == 0 ||
                           strcmp(currToken, "gt") == 0 ||
                           strcmp(currToken, "lt") == 0 ||
                           strcmp(currToken, "and") == 0 ||
                           strcmp(currToken, "or") == 0 ||
                           strcmp(currToken, "not") == 0)
                     {
                     _type = C_ARITHMETIC;
                     _arg1 = std::string(currToken);
                     }
                  else
                     {
                     _type = C_NOP;
                     }
                  memset(currToken, 0, MAX_TOKEN_SIZE);
                  currTokenSize = 0;
                  continue;
                  }
               else if (_type != C_RETURN)
                  {
                  if (_arg1.empty())
                     {
                     // set arg1
                     if (strcmp(currToken, "constant") == 0  ||
                         strcmp(currToken, "argument") == 0 ||
                         strcmp(currToken, "local") == 0 ||
                         strcmp(currToken, "pointer") == 0 ||
                         strcmp(currToken, "this") == 0 ||
                         strcmp(currToken, "that") == 0 ||
                         strcmp(currToken, "temp") == 0 ||
                         strcmp(currToken, "static") == 0)
                        {
                        _arg1 = std::string(currToken);
                        }
                     else
                        {
                        std::cout << "Error! Unexpected arg1 in line: " << _line << std::endl;
                        assert(false);
                        }
                     memset(&currToken[0], 0, MAX_TOKEN_SIZE);
                     currTokenSize = 0;
                     continue;
                     }
                  else if (!arg2Set)
                     {
                     // set arg2
                     if (_type == C_PUSH || _type == C_POP)
                        _arg2 = atoi(currToken); 
                     arg2Set = true;
                     memset(currToken, 0, MAX_TOKEN_SIZE);
                     currTokenSize = 0;
                     continue;
                     }
                  }
               std::cout << "Error! Invalid syntax encountered: " << _line << std::endl;
               assert(false);
               }
            continue;
            }
         else // regular character
            {
            if (currTokenSize < MAX_TOKEN_SIZE)
               {
               currToken[currTokenSize++] = currChar;
               }
            else
               {
               std::cout << "Error! MAX_TOKEN_SIZE too small for line: " << _line << std::endl;
               assert(false); 
               }
            }
         }
      if (_type == C_UNKNOWN)
         _type = C_NOP;   
      }
   };

class CodeWriter
   {
   private:
   std::ofstream _outputFile;
   std::string _className;
   int _counter;
   public:
   CodeWriter(std::string outputFileName): _outputFile(outputFileName), _className(outputFileName.substr(0, outputFileName.length()-4)), _counter(0) {}
   void writeArithmetic(std::string line, std::string command)
      {
      if (command == "add" || command == "sub" || command == "and" || command == "or") // D+A, D+M
         {
         _outputFile << "  @SP" << std::endl;
         _outputFile << "  M=M-1" << std::endl;
         _outputFile << "  A=M" << std::endl;
         _outputFile << "  D=M" << std::endl;
         _outputFile << "  A=A-1" << std::endl;
         if (command == "add")
            _outputFile << "  D=M+D" << std::endl;
         else if (command == "sub")
            _outputFile << "  D=M-D" << std::endl;
         else if (command == "and")
            _outputFile << "  D=M&D" << std::endl;
         else
            _outputFile << "  D=M|D" << std::endl;
         _outputFile << "  M=D" << std::endl;  
         }
      else if (command == "neg" || command == "not") // -D, -A
         {
         _outputFile << "  @SP" << std::endl;
         _outputFile << "  A=M-1" << std::endl;
         if (command == "neg")
            _outputFile << "  M=-M" << std::endl;
         else
            _outputFile << "  M=!M" << std::endl;                        
         }
      else if (command == "eq" || command == "gt" || command == "lt") // 
         {
         const char * jmp;
         if (command == "eq")
            jmp = "JEQ";
         else if (command == "gt")
            jmp = "JGT";
         else if (command == "lt")
            jmp = "JLT";
         _outputFile << "  @SP" << std::endl;
         _outputFile << "  M=M-1" << std::endl;
         _outputFile << "  A=M-1" << std::endl;
         _outputFile << "  D=M" << std::endl;
         _outputFile << "  M=-1" << std::endl;
         _outputFile << "  A=A+1" << std::endl;
         _outputFile << "  D=D-M" << std::endl;
         _outputFile << "  @LBL." << _className << _counter << std::endl;
         if (command == "eq")
            _outputFile << "  D;JEQ" << std::endl;
         else if (command == "gt")
            _outputFile << "  D;JGT" << std::endl;
         else if (command == "lt")
            _outputFile << "  D;JLT" << std::endl;
         _outputFile << "  @SP" << std::endl;
         _outputFile << "  A=M-1" << std::endl;
         _outputFile << "  M=0" << std::endl;
         _outputFile << "(LBL." << _className << _counter++ << ")" << std::endl;
         }
      else
         {
         std::cout << "Error! Invalid command encountered in CodeWriter::writeArithmetic -> " << command << std::endl;
         close();
         assert(false);
         }   
      }
   void writePushPop(std::string line, CommandType type, std::string segment, int index)
      {
      std::string segmentBase;
      if (type == C_PUSH)
         {
         // put value (from memory or constant pool) into D         
         if (segment != "pointer" && segment != "static")
            {
            _outputFile << "  @" << index << std::endl;
            _outputFile << "  D=A" << std::endl;
            }
         if (segment == "constant")
            {

            }
         else if (segment == "argument")
            {
            _outputFile << "  @ARG" << std::endl;
            _outputFile << "  A=D+M" << std::endl;
            _outputFile << "  D=M" << std::endl;
            }
         else if (segment == "this")
            {
            _outputFile << "  @THIS" << std::endl;
            _outputFile << "  A=D+M" << std::endl;
            _outputFile << "  D=M" << std::endl;
            }
         else if (segment == "that")
            {
            _outputFile << "  @THAT" << std::endl;
            _outputFile << "  A=D+M" << std::endl;
            _outputFile << "  D=M" << std::endl;
            }
         else if (segment == "pointer" && index == 0)
            {
            _outputFile << "  @THIS" << std::endl;
            _outputFile << "  D=M" << std::endl;
            }      
         else if (segment == "pointer" && index == 1)
            {
            _outputFile << "  @THAT" << std::endl;
            _outputFile << "  D=M" << std::endl;
            }  
         else if (segment == "temp")
            {
            _outputFile << "  @R" << 5 + index << std::endl;
            _outputFile << "  D=M" << std::endl;
            }   
         else if (segment == "local")
            {
            _outputFile << "  @LCL" << std::endl;
            _outputFile << "  A=D+M" << std::endl;
            _outputFile << "  D=M" << std::endl;
            }
         else if (segment == "static")
            {
            _outputFile << "  @" << _className << index << std::endl;
            _outputFile << "  D=M" << std::endl;
            }
         else
            {
            std::cout << "Error! Invalid segment encountered in CodeWriter::writePushPop -> " << segment << std::endl;
            close();
            assert(false);
            }
         // RAM[SP++] = D
         _outputFile << "  @SP" << std::endl;
         _outputFile << "  M=M+1" << std::endl;
         _outputFile << "  A=M-1" << std::endl;
         _outputFile << "  M=D" << std::endl;
         }
      else if (type == C_POP)
         {
         // put address location into R13
         if (segment != "pointer" && segment != "static")
            {
            _outputFile << "  @" << index << std::endl;
            _outputFile << "  D=A" << std::endl;
            }
         if (segment == "argument")
            {
            _outputFile << "  @ARG" << std::endl;
            _outputFile << "  D=D+M" << std::endl;
            _outputFile << "  @R13" << std::endl;
            _outputFile << "  M=D" << std::endl;            
            }
         else if (segment == "this")
            {
            _outputFile << "  @THIS" << std::endl;
            _outputFile << "  D=D+M" << std::endl;
            _outputFile << "  @R13" << std::endl;
            _outputFile << "  M=D" << std::endl;            
            }
         else if (segment == "that")
            {
            _outputFile << "  @THAT" << std::endl;
            _outputFile << "  D=D+M" << std::endl;
            _outputFile << "  @R13" << std::endl;
            _outputFile << "  M=D" << std::endl;            
            }
         else if (segment == "pointer" && index == 0)
            {
            _outputFile << "  @THIS" << std::endl;
            _outputFile << "  D=A" << std::endl;
            _outputFile << "  @R13" << std::endl;
            _outputFile << "  M=D" << std::endl;            

            }      
         else if (segment == "pointer" && index == 1)
            {
            _outputFile << "  @THAT" << std::endl;
            _outputFile << "  D=A" << std::endl;
            _outputFile << "  @R13" << std::endl;
            _outputFile << "  M=D" << std::endl;            

            }  
         else if (segment == "local")
            {
            _outputFile << "  @LCL" << std::endl;
            _outputFile << "  D=D+M" << std::endl;
            _outputFile << "  @R13" << std::endl;
            _outputFile << "  M=D" << std::endl;
            }
         else if (segment == "temp")
            {
               
            }
         else if (segment == "static")
            {
            _outputFile << "  @" << _className << index << std::endl;
            _outputFile << "  D=A" << std::endl;
            _outputFile << "  @R13" << std::endl;
            _outputFile << "  M=D" << std::endl;               
            }
         else
            {
            std::cout << "Error! Invalid segment encountered in CodeWriter::writePushPop -> " << segment << std::endl;
            close();
            assert(false);
            }
         _outputFile << "  @SP" << std::endl;
         _outputFile << "  M=M-1" << std::endl;
         _outputFile << "  A=M" << std::endl;
         _outputFile << "  D=M" << std::endl;
         if (segment == "temp")
            {
            _outputFile << "  @R" << 5+index << std::endl;
            }
         else
            {
            _outputFile << "  @R13" << std::endl;
            _outputFile << "  A=M" << std::endl;
            }        
         _outputFile << "  M=D" << std::endl;
         }
      else
         {
         std::cout << "Error! Invalid command encountered in CodeWriter::writePushPop -> " << type << std::endl;
         close();
         assert(false);
         }
      }
   void writeComment(std::string line) {_outputFile << "// " << line << std::endl;}
   void close() {_outputFile.close();}
   };

int main(int argc, char* argv[])
   {
   std::string inputFileName(argv[1]);
   std::string outputFileName = inputFileName.substr(0, inputFileName.length()-2) + "asm";
   Parser myParser(inputFileName);
   CodeWriter myCodeWriter(outputFileName);
   for (myParser.advance(); myParser.hasMoreCommands(); myParser.advance())
      {
      myCodeWriter.writeComment(myParser.getLine());
      switch (myParser.commandType())
         {
         case C_PUSH:
         case C_POP:
            myCodeWriter.writePushPop(myParser.getLine(), myParser.commandType(), myParser.arg1(), myParser.arg2());
            break;
         case C_ARITHMETIC:
            myCodeWriter.writeArithmetic(myParser.getLine(), myParser.arg1());
            break;
         case C_NOP:
            break;
         default:
            std::cout << "Error! Invalid command type encountered." << std::endl;
            myCodeWriter.close();
            assert(false);
         }
      }
   myCodeWriter.close();
   return 0;         
   }
