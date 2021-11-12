#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <assert.h>
#include <stack>  

enum TokenType {T_KEYWORD, T_SYMBOL, T_IDENTIFIER,  T_INT_CONST, T_STRING_CONST};
enum KeywordType {K_CLASS, K_METHOD, K_FUNCTION, K_CONSTRUCTOR, K_INT, K_BOOLEAN, K_CHAR, K_VOID, K_VAR, K_STATIC, K_FIELD, K_LET, K_DO, K_IF, K_ELSE, K_WHILE, K_RETURN, K_TRUE, K_FALSE, K_NULL, K_THIS};

class JackToken
   {
   public:
   std::string _text;
   TokenType _token_type;
   KeywordType _keyword_type;
   TokenType getTokenType()
      {
	   return _token_type;
      }
   KeywordType getKeyword()
      {
	   return _keyword_type;
      }
   char getSymbol()
      {
	   return _text[0]; 
      }
   std::string getIdentifier()
      {
	   return _text; 
      }
   int getIntVal()
      {
	   return std::stoi(_text);
      }
   std::string getStringVal()
      {
	    return _text;
      }
   };


class JackTokenizer
   {
   private:
   std::ifstream _inputFile;
   JackToken _currToken;
   std::unordered_map<std::string, KeywordType> _str_to_keyword_map;

   bool isWhitespace(char c)
      {
      return c == ' ' || c == '\n' || c == '\r';
      }

   bool isSymbol(char c)
      {
      return c == '{' || c == '}' || c == '[' || c == ']' || c == '(' || c == ')' ||
             c == '.' || c == ',' || c == ';' || c == '+' || c == '-' || c == '*' ||
             c == '/' || c == '&' || c == '|' || c == '<' || c == '>' || c == '=' || c == '~';
      }
   
   void skipWhitespace()
      {
      bool inside_comment = false;
      bool multiline_comment = false;
	   while (true)
	      {
         char byte1 = _inputFile.peek();
         char byte2;
         if (byte1 == EOF)
            break;
         else
            {
            byte1 = _inputFile.get();
            byte2 = _inputFile.peek();
            _inputFile.putback(byte1);
            }
         if (byte2 == EOF)
            {
            if (inside_comment || byte1 == '\n' || byte1 == '\r' || byte1 == ' ')
               _inputFile.get();
            break;
            }
         //printf("byte1 %c, byte2 %c inside_comment %d\n", byte1, byte2, inside_comment);
         
         if (inside_comment)
            {
            if (byte1 == '*' && byte2 == '/')
               {
               _inputFile.get();
               inside_comment = false;
               }
            else if (!multiline_comment && (byte1 == '\n' || byte1 == '\r'))
               inside_comment = false;
            _inputFile.get();
            }
         else if (byte1 == '/' && (byte2 == '/' || byte2 == '*'))
            {
            _inputFile.get();
            _inputFile.get();
            inside_comment = true;
            multiline_comment = byte2 == '*';
            }
         else if (byte1 == ' ' || byte1 == '\n' || byte1 == '\r' || byte1 == '\t')
            _inputFile.get();
         else
            break;
	      }
      }
   public:
   JackTokenizer(std::filesystem::path inputFileName): _inputFile(inputFileName)
      {
      _str_to_keyword_map.insert(std::make_pair("class", K_CLASS));
      _str_to_keyword_map.insert(std::make_pair("constructor", K_CONSTRUCTOR));
      _str_to_keyword_map.insert(std::make_pair("function", K_FUNCTION));
      _str_to_keyword_map.insert(std::make_pair("method", K_METHOD));
      _str_to_keyword_map.insert(std::make_pair("field", K_FIELD));
      _str_to_keyword_map.insert(std::make_pair("static", K_STATIC));
      _str_to_keyword_map.insert(std::make_pair("var", K_VAR));
      _str_to_keyword_map.insert(std::make_pair("int", K_INT));
      _str_to_keyword_map.insert(std::make_pair("char", K_CHAR));
      _str_to_keyword_map.insert(std::make_pair("boolean", K_BOOLEAN));
      _str_to_keyword_map.insert(std::make_pair("void", K_VOID));
      _str_to_keyword_map.insert(std::make_pair("true", K_TRUE));
      _str_to_keyword_map.insert(std::make_pair("false", K_FALSE));
      _str_to_keyword_map.insert(std::make_pair("null", K_NULL));
      _str_to_keyword_map.insert(std::make_pair("this", K_THIS));
      _str_to_keyword_map.insert(std::make_pair("let", K_LET));
      _str_to_keyword_map.insert(std::make_pair("do", K_DO));
      _str_to_keyword_map.insert(std::make_pair("if", K_IF));
      _str_to_keyword_map.insert(std::make_pair("else", K_ELSE));
      _str_to_keyword_map.insert(std::make_pair("while", K_WHILE));
      _str_to_keyword_map.insert(std::make_pair("return", K_RETURN));
      if (hasMoreTokens())
         advance();
      }   
   JackToken getCurrentToken()
      {
	   return _currToken;
      }
   bool hasMoreTokens()
      {
	   skipWhitespace();
      if (_inputFile.peek() == EOF)
         return false;
      return true;
      }
   void advance()
      {
      skipWhitespace();
      std::string text;
      TokenType token_type;
      KeywordType keyword_type;            
      char byte = _inputFile.get();
      
      text.push_back(byte);
      //printf("advance, token: %s\n", text.c_str());
      if (isSymbol(byte))
         token_type = T_SYMBOL;
      else
         {
         if (byte == '"')
            {
            token_type = T_STRING_CONST;
            text.pop_back();
            }
         else if (byte - '0' <= 9 && byte - '0' >= 0)
            token_type = T_INT_CONST;
         else
            token_type = T_IDENTIFIER;
         byte = _inputFile.peek();
         while (byte != EOF && (token_type == T_STRING_CONST || (!isWhitespace(byte) && !isSymbol(byte))) /*&& byte != '"'*/)
            {
            if (byte == '"')
               {
               if (token_type == T_STRING_CONST)
                  byte = _inputFile.get();
               break;
               }
            text.push_back(_inputFile.get());
            byte = _inputFile.peek();
            }
         if (_str_to_keyword_map.find(text) != _str_to_keyword_map.end())
            {
            token_type = T_KEYWORD;
            keyword_type = _str_to_keyword_map.find(text)->second;
            }
         }
      //printf("advance end, token: %s\n", text.c_str());
      _currToken._text = text;
      _currToken._token_type = token_type;
      _currToken._keyword_type = keyword_type;
      }
   };

class CompilationEngine
   {
   private:
   std::ofstream _outputFile;
   std::string _className;
   JackTokenizer * _tokenizer;
   int _spaces;
   void eat(TokenType tt, std::string text)
      {
      //printf("eat %s\n", text.c_str());
      //printf("text %s\n", _tokenizer->getCurrentToken()._text.c_str());
      if (_tokenizer->getCurrentToken().getTokenType() != tt)
         assert(false);
      if (_tokenizer->getCurrentToken()._text != text)
         assert(false);
      _tokenizer->advance();
      }
   
   public:
   CompilationEngine(JackTokenizer * t, std::string ofstr): _tokenizer(t), _outputFile(ofstr), _spaces(0) {}
   void compileClass()
      {
      _outputFile << std::string( _spaces, ' ' ) << "<class>" << std::endl;
      _spaces+=2;
      //printf("curr token text is %s\n", _tokenizer->getCurrentToken()._text.c_str());
      eat(T_KEYWORD, "class");
      _outputFile << std::string( _spaces, ' ' ) << "<keyword> class </keyword>" << std::endl;
      
      
      TokenType tt = _tokenizer->getCurrentToken().getTokenType();
      if (_tokenizer->getCurrentToken().getTokenType() == T_IDENTIFIER)
         _outputFile << std::string( _spaces, ' ' ) << "<identifier> " << _tokenizer->getCurrentToken()._text << " </identifier>" << std::endl;
      else
         assert(false);
      _tokenizer->advance();
      eat(T_SYMBOL, "{");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> { </symbol>" << std::endl;
      
      while (_tokenizer->getCurrentToken().getTokenType() == T_KEYWORD)
         {
         if (_tokenizer->getCurrentToken()._text == "static" ||
             _tokenizer->getCurrentToken()._text == "field")
            {
            compileClassVarDec();
            _tokenizer->advance();
            }
         else
            break;
         }
      ////printf("done with fields...\n");         
      while (_tokenizer->getCurrentToken().getTokenType() == T_KEYWORD)
         {
         if (_tokenizer->getCurrentToken()._text == "constructor" ||
             _tokenizer->getCurrentToken()._text == "function" ||
             _tokenizer->getCurrentToken()._text == "method")
            {
            compileSubroutine();
            //printf("after compileSubroutine token is... %s\n", _tokenizer->getCurrentToken()._text.c_str());
            //_tokenizer->advance();
            }
         else
            break;
         }
      eat(T_SYMBOL, "}");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> } </symbol>" << std::endl;
      
      while (_tokenizer->hasMoreTokens())
         {
         _tokenizer->advance();
         }         
      
      _spaces-=2;
      _outputFile << std::string( _spaces, ' ' ) << "</class>" << std::endl;
      }
   void compileClassVarDec()
      {
      //printf("compileClassVarDec: curr token text is %s\n", _tokenizer->getCurrentToken()._text.c_str());
      _outputFile << std::string( _spaces, ' ' ) << "<classVarDec>" << std::endl;
      _spaces+=2;
      if (_tokenizer->getCurrentToken().getTokenType() == T_KEYWORD &&
          _tokenizer->getCurrentToken()._text == "static" ||
          _tokenizer->getCurrentToken()._text == "field")
         _outputFile << std::string( _spaces, ' ' ) << "<keyword> " << _tokenizer->getCurrentToken()._text << " </keyword>" << std::endl;
      else
         assert(false);
      _tokenizer->advance();
      if (_tokenizer->getCurrentToken().getTokenType() == T_KEYWORD &&
          _tokenizer->getCurrentToken()._text == "int" ||
          _tokenizer->getCurrentToken()._text == "boolean" ||
          _tokenizer->getCurrentToken()._text == "char")
         _outputFile << std::string( _spaces, ' ' ) << "<keyword> " << _tokenizer->getCurrentToken()._text << " </keyword>" << std::endl;
      else if (_tokenizer->getCurrentToken().getTokenType() == T_IDENTIFIER)
         _outputFile << std::string( _spaces, ' ' ) << "<identifier> " << _tokenizer->getCurrentToken()._text << " </identifier>" << std::endl;
      else
         assert(false);
      _tokenizer->advance();
      while (_tokenizer->hasMoreTokens())
         {
         if (_tokenizer->getCurrentToken().getTokenType() == T_IDENTIFIER)
            _outputFile << std::string( _spaces, ' ' ) << "<identifier> " << _tokenizer->getCurrentToken()._text << " </identifier>" << std::endl;
         else
            assert(false);
         _tokenizer->advance();
         //printf("next token is... %s\n", _tokenizer->getCurrentToken()._text.c_str());
         if (_tokenizer->getCurrentToken().getTokenType() == T_SYMBOL)
            {
            if (_tokenizer->getCurrentToken()._text == ";")
               break;
            else if (_tokenizer->getCurrentToken()._text == ",")
               _outputFile << std::string( _spaces, ' ' ) << "<symbol> , </symbol>" << std::endl;
            }
         else
            assert(false);
         _tokenizer->advance();
         }
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> ; </symbol>" << std::endl;
      _spaces-=2;
      _outputFile << std::string( _spaces, ' ' ) << "</classVarDec>" << std::endl;
      }
   void compileSubroutine()
      {
      _outputFile << std::string( _spaces, ' ' ) << "<subroutineDec>" << std::endl;
      _spaces+=2;
      //printf("subRoutineDec token is... %s\n", _tokenizer->getCurrentToken()._text.c_str());
      if (_tokenizer->getCurrentToken().getTokenType() == T_KEYWORD &&
          _tokenizer->getCurrentToken()._text == "constructor" ||
          _tokenizer->getCurrentToken()._text == "function" ||
          _tokenizer->getCurrentToken()._text == "method")
         _outputFile << std::string( _spaces, ' ' ) << "<keyword> " << _tokenizer->getCurrentToken()._text << " </keyword>" << std::endl;
      else
         assert(false);
      _tokenizer->advance();
      if (_tokenizer->getCurrentToken().getTokenType() == T_KEYWORD &&
          _tokenizer->getCurrentToken()._text == "void" ||
          _tokenizer->getCurrentToken()._text == "int" ||
          _tokenizer->getCurrentToken()._text == "boolean" ||
          _tokenizer->getCurrentToken()._text == "char")
         _outputFile << std::string( _spaces, ' ' ) << "<keyword> " << _tokenizer->getCurrentToken()._text << " </keyword>" << std::endl;
      else if (_tokenizer->getCurrentToken().getTokenType() == T_IDENTIFIER)
         _outputFile << std::string( _spaces, ' ' ) << "<identifier> " << _tokenizer->getCurrentToken()._text << " </identifier>" << std::endl;   
      else      
         assert(false);      
      _tokenizer->advance();
      if (_tokenizer->getCurrentToken().getTokenType() == T_IDENTIFIER)
         _outputFile << std::string( _spaces, ' ' ) << "<identifier> " << _tokenizer->getCurrentToken()._text << " </identifier>" << std::endl;      
      else
         assert(false);
      _tokenizer->advance();
      eat(T_SYMBOL, "(");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> ( </symbol>" << std::endl;
      compileParameterList();
      eat(T_SYMBOL, ")");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> ) </symbol>" << std::endl;
      compileSubroutineBody();
      _spaces-=2;
      _outputFile << std::string( _spaces, ' ' ) << "</subroutineDec>" << std::endl;
      }   
   void compileParameterList()
      {
      _outputFile << std::string( _spaces, ' ' ) << "<parameterList>" << std::endl;
      //printf("compileParameterList token is... %s\n", _tokenizer->getCurrentToken()._text.c_str());
      while (_tokenizer->hasMoreTokens() && _tokenizer->getCurrentToken().getTokenType() != T_SYMBOL)
         {
         if (_tokenizer->getCurrentToken().getTokenType() == T_KEYWORD &&
             _tokenizer->getCurrentToken()._text == "void" ||
             _tokenizer->getCurrentToken()._text == "int" ||
             _tokenizer->getCurrentToken()._text == "boolean" ||
             _tokenizer->getCurrentToken()._text == "char")
            _outputFile << std::string( _spaces, ' ' ) << "<keyword> " << _tokenizer->getCurrentToken()._text << " </keyword>" << std::endl;
         else if (_tokenizer->getCurrentToken().getTokenType() == T_IDENTIFIER)
            _outputFile << std::string( _spaces, ' ' ) << "<identifier> " << _tokenizer->getCurrentToken()._text << " </identifier>" << std::endl;
         else
            assert(false);
         _tokenizer->advance();
         if (_tokenizer->getCurrentToken().getTokenType() == T_IDENTIFIER)
            _outputFile << std::string( _spaces, ' ' ) << "<identifier> " << _tokenizer->getCurrentToken()._text << " </identifier>" << std::endl;
         else
            assert(false);
         _tokenizer->advance();
         if (_tokenizer->getCurrentToken().getTokenType() == T_SYMBOL &&
             _tokenizer->getCurrentToken()._text == ",")
            _outputFile << std::string( _spaces, ' ' ) << "<symbol> , </symbol>" << std::endl;
         else
            break;
         _tokenizer->advance();
         }
      //_tokenizer->advance();
      _outputFile << std::string( _spaces, ' ' ) << "</parameterList>" << std::endl;
      }
   void compileSubroutineBody()
      {
      _outputFile << std::string( _spaces, ' ' ) << "<subroutineBody>" << std::endl;
      _spaces+=2;
      //printf("compileSubroutineBody token is... %s\n", _tokenizer->getCurrentToken()._text.c_str());
      eat(T_SYMBOL, "{");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> { </symbol>" << std::endl;      
      while (_tokenizer->hasMoreTokens() &&
             _tokenizer->getCurrentToken().getTokenType() == T_KEYWORD &&
             _tokenizer->getCurrentToken()._text == "var")
         {
         compileVarDec();
         }
      compileStatements();
      eat(T_SYMBOL, "}");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> } </symbol>" << std::endl;
      _spaces-=2;
      _outputFile << std::string( _spaces, ' ' ) << "</subroutineBody>" << std::endl;
      }
   void compileVarDec()
      {
      _outputFile << std::string( _spaces, ' ' ) << "<varDec>" << std::endl;
      _spaces+=2;
      //printf("compileVarDec token is... %s\n", _tokenizer->getCurrentToken()._text.c_str());
      eat(T_KEYWORD, "var");
      _outputFile << std::string( _spaces, ' ' ) << "<keyword> var </keyword>" << std::endl;
      while (_tokenizer->hasMoreTokens() && _tokenizer->getCurrentToken().getTokenType() != T_SYMBOL)
         {
         if (_tokenizer->getCurrentToken().getTokenType() == T_KEYWORD &&
             _tokenizer->getCurrentToken()._text == "void" ||
             _tokenizer->getCurrentToken()._text == "int" ||
             _tokenizer->getCurrentToken()._text == "boolean" ||
             _tokenizer->getCurrentToken()._text == "char")
            _outputFile << std::string( _spaces, ' ' ) << "<keyword> " << _tokenizer->getCurrentToken()._text << " </keyword>" << std::endl;
         else if (_tokenizer->getCurrentToken().getTokenType() == T_IDENTIFIER)
            _outputFile << std::string( _spaces, ' ' ) << "<identifier> " << _tokenizer->getCurrentToken()._text << " </identifier>" << std::endl;
         else
            assert(false);
         _tokenizer->advance();
         if (_tokenizer->getCurrentToken().getTokenType() == T_IDENTIFIER)
            _outputFile << std::string( _spaces, ' ' ) << "<identifier> " << _tokenizer->getCurrentToken()._text << " </identifier>" << std::endl;
         else if (_tokenizer->getCurrentToken().getTokenType() == T_SYMBOL &&
             _tokenizer->getCurrentToken()._text == ";")
             break;
         else
            assert(false);
         _tokenizer->advance();
         if (_tokenizer->getCurrentToken().getTokenType() == T_SYMBOL &&
             _tokenizer->getCurrentToken()._text == ",")
            _outputFile << std::string( _spaces, ' ' ) << "<symbol> , </symbol>" << std::endl;
         else
            break;
         _tokenizer->advance();
         }      
      eat(T_SYMBOL, ";");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> ; </symbol>" << std::endl;
      _spaces-=2;
      _outputFile << std::string( _spaces, ' ' ) << "</varDec>" << std::endl;
      }
   void compileStatements()
      {
      _outputFile << std::string( _spaces, ' ' ) << "<statements>" << std::endl;
      _spaces+=2;
      //printf("compileStatements token is... %s\n", _tokenizer->getCurrentToken()._text.c_str());
      while (_tokenizer->hasMoreTokens() && _tokenizer->getCurrentToken().getTokenType() == T_KEYWORD)
         {
         switch(_tokenizer->getCurrentToken()._keyword_type)
            {
            case K_LET:
               compileLet();
               break;
            case K_DO:
               compileDo();
               break;
            case K_RETURN:
               compileReturn();
               break;
            case K_IF:
               compileIf();
               break;
            case K_WHILE:
               compileWhile();
               break;
            }
         //_tokenizer->advance();
         }
      _spaces-=2;
      _outputFile << std::string( _spaces, ' ' ) << "</statements>" << std::endl;
      }
   void compileLet()
      {
      _outputFile << std::string( _spaces, ' ' ) << "<letStatement>" << std::endl;
      _spaces+=2;
      eat(T_KEYWORD, "let");
      _outputFile << std::string( _spaces, ' ' ) << "<keyword> let </keyword>" << std::endl;
      if (_tokenizer->getCurrentToken().getTokenType() == T_IDENTIFIER)
         _outputFile << std::string( _spaces, ' ' ) << "<identifier> " << _tokenizer->getCurrentToken()._text << " </identifier>" << std::endl;  
      else
         assert(false);
      _tokenizer->advance();
      if (_tokenizer->getCurrentToken().getTokenType() == T_SYMBOL &&
          _tokenizer->getCurrentToken()._text == "[")
         {
         eat(T_SYMBOL, "[");
         _outputFile << std::string( _spaces, ' ' ) << "<symbol> [ </symbol>" << std::endl;
         compileExpression();
         eat(T_SYMBOL, "]");
         _outputFile << std::string( _spaces, ' ' ) << "<symbol> ] </symbol>" << std::endl;
         }
      eat(T_SYMBOL, "=");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> = </symbol>" << std::endl;
      compileExpression();
      //printf("compileLet token is... %s\n", _tokenizer->getCurrentToken()._text.c_str());
      eat(T_SYMBOL, ";");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> ; </symbol>" << std::endl;
      _spaces-=2;
      _outputFile << std::string( _spaces, ' ' ) << "</letStatement>" << std::endl;
      }
   void compileIf()
      {
      _outputFile << std::string( _spaces, ' ' ) << "<ifStatement>" << std::endl;
      _spaces+=2;
      eat(T_KEYWORD, "if");
      _outputFile << std::string( _spaces, ' ' ) << "<keyword> if </keyword>" << std::endl;
      eat(T_SYMBOL, "(");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> ( </symbol>" << std::endl;      
      compileExpression();
      eat(T_SYMBOL, ")");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> ) </symbol>" << std::endl;
      eat(T_SYMBOL, "{");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> { </symbol>" << std::endl;      
      compileStatements();
      eat(T_SYMBOL, "}");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> } </symbol>" << std::endl;
      if (_tokenizer->getCurrentToken().getTokenType() == T_KEYWORD &&
         _tokenizer->getCurrentToken()._text == "else")
         {
         _outputFile << std::string( _spaces, ' ' ) << "<keyword> else </keyword>" << std::endl;
         _tokenizer->advance();
         eat(T_SYMBOL, "{");
         _outputFile << std::string( _spaces, ' ' ) << "<symbol> { </symbol>" << std::endl;      
         compileStatements();
         eat(T_SYMBOL, "}");   
         _outputFile << std::string( _spaces, ' ' ) << "<symbol> } </symbol>" << std::endl;             
         }
      _spaces-=2;
      _outputFile << std::string( _spaces, ' ' ) << "</ifStatement>" << std::endl;
      }
   void compileWhile()
      {
      _outputFile << std::string( _spaces, ' ' ) << "<whileStatement>" << std::endl;
      _spaces+=2;
      eat(T_KEYWORD, "while");
      _outputFile << std::string( _spaces, ' ' ) << "<keyword> while </keyword>" << std::endl;
      eat(T_SYMBOL, "(");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> ( </symbol>" << std::endl;      
      compileExpression();
      eat(T_SYMBOL, ")");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> ) </symbol>" << std::endl;
      eat(T_SYMBOL, "{");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> { </symbol>" << std::endl;      
      compileStatements();
      eat(T_SYMBOL, "}");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> } </symbol>" << std::endl;
      _spaces-=2;
      _outputFile << std::string( _spaces, ' ' ) << "</whileStatement>" << std::endl;      
      }
   void compileDo()
      {
      _outputFile << std::string( _spaces, ' ' ) << "<doStatement>" << std::endl;
      _spaces+=2;
      eat(T_KEYWORD, "do");
      _outputFile << std::string( _spaces, ' ' ) << "<keyword> do </keyword>" << std::endl;
      if (_tokenizer->getCurrentToken().getTokenType() == T_IDENTIFIER)
         _outputFile << std::string( _spaces, ' ' ) << "<identifier> " << _tokenizer->getCurrentToken()._text << " </identifier>" << std::endl;  
      else
         assert(false);
      _tokenizer->advance();
      if (_tokenizer->getCurrentToken().getTokenType() == T_SYMBOL && _tokenizer->getCurrentToken()._text == "(")
         {
         _outputFile << std::string( _spaces, ' ' ) << "<symbol> ( </symbol>" << std::endl;
         _tokenizer->advance();
         compileExpressionList();
         eat(T_SYMBOL, ")");
         _outputFile << std::string( _spaces, ' ' ) << "<symbol> ) </symbol>" << std::endl;
         }
      else if (_tokenizer->getCurrentToken().getTokenType() == T_SYMBOL && _tokenizer->getCurrentToken()._text == ".")
         {
         _outputFile << std::string( _spaces, ' ' ) << "<symbol> . </symbol>" << std::endl;
         _tokenizer->advance();
         if (_tokenizer->getCurrentToken().getTokenType() == T_IDENTIFIER)
            _outputFile << std::string( _spaces, ' ' ) << "<identifier> " << _tokenizer->getCurrentToken()._text << " </identifier>" << std::endl;  
         else
            assert(false);
         _tokenizer->advance();         
         eat(T_SYMBOL, "(");
         _outputFile << std::string( _spaces, ' ' ) << "<symbol> ( </symbol>" << std::endl;
         compileExpressionList();
         eat(T_SYMBOL, ")");
         _outputFile << std::string( _spaces, ' ' ) << "<symbol> ) </symbol>" << std::endl;
         }
      else
         assert(false);
      
      //compileExpression();
      eat(T_SYMBOL, ";");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> ; </symbol>" << std::endl;
      _spaces-=2;
      _outputFile << std::string( _spaces, ' ' ) << "</doStatement>" << std::endl;      
      }
   void compileReturn()
      {
      _outputFile << std::string( _spaces, ' ' ) << "<returnStatement>" << std::endl;
      eat(T_KEYWORD, "return");
      _outputFile << std::string( _spaces, ' ' ) << "<keyword> return </keyword>" << std::endl;
      if (!(_tokenizer->getCurrentToken().getTokenType() == T_SYMBOL &&
          _tokenizer->getCurrentToken()._text == ";"))
         {
         compileExpression();
         }
      eat(T_SYMBOL, ";");
      _outputFile << std::string( _spaces, ' ' ) << "<symbol> ; </symbol>" << std::endl;
      _outputFile << std::string( _spaces, ' ' ) << "</returnStatement>" << std::endl;
      }
   void compileExpression()
      {
      //printf("compileExpression current token1 is %s\n", _tokenizer->getCurrentToken()._text.c_str());
      if (_tokenizer->getCurrentToken().getTokenType() == T_IDENTIFIER ||
          _tokenizer->getCurrentToken().getTokenType() == T_KEYWORD ||
          _tokenizer->getCurrentToken().getTokenType() == T_STRING_CONST ||
          _tokenizer->getCurrentToken().getTokenType() == T_INT_CONST ||
          ((_tokenizer->getCurrentToken().getTokenType() == T_SYMBOL &&
            _tokenizer->getCurrentToken()._text == "-" ||
            _tokenizer->getCurrentToken()._text == "~" ||
            _tokenizer->getCurrentToken()._text == "(")))
         {
         _outputFile << std::string( _spaces, ' ' ) << "<expression>" << std::endl;
         _spaces+=2;
         compileTerm();
         //printf("current token2 is %s\n", _tokenizer->getCurrentToken()._text.c_str());
         while (_tokenizer->getCurrentToken().getTokenType() == T_SYMBOL &&
                _tokenizer->getCurrentToken()._text == "+" ||
                _tokenizer->getCurrentToken()._text == "-" ||
                _tokenizer->getCurrentToken()._text == "*" ||
                _tokenizer->getCurrentToken()._text == "/" ||
                _tokenizer->getCurrentToken()._text == "&" ||
                _tokenizer->getCurrentToken()._text == "|" ||
                _tokenizer->getCurrentToken()._text == "<" ||
                _tokenizer->getCurrentToken()._text == ">" ||
                _tokenizer->getCurrentToken()._text == "=")       
             {
             //eat(T_SYMBOL, "+");
             _outputFile << std::string( _spaces, ' ' ) << "<symbol> " << _tokenizer->getCurrentToken()._text << " </symbol>" << std::endl;
             _tokenizer->advance();
             compileTerm();
             //printf("current token3 is %s\n", _tokenizer->getCurrentToken()._text.c_str());
             }
         _spaces-=2;
         _outputFile << std::string( _spaces, ' ' ) << "</expression>" << std::endl;
         }
         /*
      else if (_tokenizer->getCurrentToken().getTokenType() == T_SYMBOL &&
               _tokenizer->getCurrentToken()._text == "-" || _tokenizer->getCurrentToken()._text == "~" ||
               _tokenizer->getCurrentToken()._text == "(")
         {
         _outputFile << std::string( _spaces, ' ' ) << "<expression>" << std::endl;
         _spaces+=2;
         compileTerm();
         _spaces-=2;
         _outputFile << std::string( _spaces, ' ' ) << "</expression>" << std::endl;
         }
         */
      
      }
   void compileTerm()
      {
      _outputFile << std::string( _spaces, ' ' ) << "<term>" << std::endl;
      _spaces+=2;
      if (_tokenizer->getCurrentToken().getTokenType() == T_IDENTIFIER)
         {
         _outputFile << std::string( _spaces, ' ' ) << "<identifier> " << _tokenizer->getCurrentToken()._text << " </identifier>" << std::endl;
         _tokenizer->advance();
         if (_tokenizer->getCurrentToken().getTokenType() == T_SYMBOL && _tokenizer->getCurrentToken()._text == ".")
            {
            // subroutine call
            eat(T_SYMBOL, ".");
            _outputFile << std::string( _spaces, ' ' ) << "<symbol> . </symbol>" << std::endl;
            if (_tokenizer->getCurrentToken().getTokenType() == T_IDENTIFIER)
               {
               _outputFile << std::string( _spaces, ' ' ) << "<identifier> " << _tokenizer->getCurrentToken()._text << " </identifier>" << std::endl;
               _tokenizer->advance(); 
               }
            else
               assert(false);
            eat(T_SYMBOL, "(");
            _outputFile << std::string( _spaces, ' ' ) << "<symbol> ( </symbol>" << std::endl;
            compileExpressionList();
            eat(T_SYMBOL, ")");
            _outputFile << std::string( _spaces, ' ' ) << "<symbol> ) </symbol>" << std::endl;
            }
         else if (_tokenizer->getCurrentToken().getTokenType() == T_SYMBOL && _tokenizer->getCurrentToken()._text == "[")
            {
            _outputFile << std::string( _spaces, ' ' ) << "<symbol> [ </symbol>" << std::endl;
            _tokenizer->advance();
            compileExpression();
            eat(T_SYMBOL, "]");
            _outputFile << std::string( _spaces, ' ' ) << "<symbol> ] </symbol>" << std::endl;
            }
         }
      else if (_tokenizer->getCurrentToken().getTokenType() == T_KEYWORD)
         {
         _outputFile << std::string( _spaces, ' ' ) << "<keyword> " << _tokenizer->getCurrentToken()._text << " </keyword>" << std::endl;
         _tokenizer->advance();
         }
      else if (_tokenizer->getCurrentToken().getTokenType() == T_STRING_CONST)
         {
         _outputFile << std::string( _spaces, ' ' ) << "<stringConstant> " << _tokenizer->getCurrentToken()._text << " </stringConstant>" << std::endl;
         _tokenizer->advance();
         }
      else if (_tokenizer->getCurrentToken().getTokenType() == T_INT_CONST)
         {
         _outputFile << std::string( _spaces, ' ' ) << "<integerConstant> " << _tokenizer->getCurrentToken()._text << " </integerConstant>" << std::endl;
         _tokenizer->advance();
         }
      else if (_tokenizer->getCurrentToken().getTokenType() == T_SYMBOL &&
               _tokenizer->getCurrentToken()._text == "-" || _tokenizer->getCurrentToken()._text == "~")
         {
         _outputFile << std::string( _spaces, ' ' ) << "<symbol> " << _tokenizer->getCurrentToken()._text << " </symbol>" << std::endl;
         _tokenizer->advance();
         compileTerm();
         }
      else if (_tokenizer->getCurrentToken().getTokenType() == T_SYMBOL)
         {
         eat(T_SYMBOL, "(");
         _outputFile << std::string( _spaces, ' ' ) << "<symbol> ( </symbol>" << std::endl;
         compileExpression();
         eat(T_SYMBOL, ")");
         _outputFile << std::string( _spaces, ' ' ) << "<symbol> ) </symbol>" << std::endl;
         }      

      _spaces-=2;
      _outputFile << std::string( _spaces, ' ' ) << "</term>" << std::endl;      
      }
   void compileExpressionList()
      {
      _outputFile << std::string( _spaces, ' ' ) << "<expressionList>" << std::endl;
      _spaces+=2;
      compileExpression();
      while (_tokenizer->getCurrentToken().getTokenType() == T_SYMBOL && _tokenizer->getCurrentToken()._text == ",")
         {
         eat(T_SYMBOL, ",");
         _outputFile << std::string( _spaces, ' ' ) << "<symbol> , </symbol>" << std::endl;
         compileExpression();
         }
      _spaces-=2;
      _outputFile << std::string( _spaces, ' ' ) << "</expressionList>" << std::endl;
      }
   };


void tokenizeAndCompileFile(std::string input_file_str, std::string output_file_str)
   {
   JackTokenizer myJackTokenizer(input_file_str);
   CompilationEngine myCompilationEngine(&myJackTokenizer, output_file_str);
   while (myJackTokenizer.hasMoreTokens())
      myCompilationEngine.compileClass();
   }

int main(int argc, char* argv[])
   {
   const std::filesystem::path path(argv[1]);
   if (std::filesystem::is_directory(path))
      {
      for (const auto & entry : std::filesystem::directory_iterator(path))
         {
         if (entry.path().extension() == ".jack")
            tokenizeAndCompileFile(entry.path().string(), entry.path().stem().string() + ".xml");
         }
      }
   else if (std::filesystem::is_regular_file(path) && path.extension() == ".jack")
      {
      tokenizeAndCompileFile(path.string(), path.stem().string() + ".xml");
      }
   return 0;         
   }
   
