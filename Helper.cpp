#ifndef HELPER_H
#define HELPER_H

#include "IHelper.h"

#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Helper : public IHelper{
  public:
    Helper(){
      this->cont = 0;  // Tratar numeracao de loops
    }

    bool startCode(string filename = "main.asm"){
      ofstream assembly;
      assembly.open(filename);

      assembly << ".org 0x00\n";
      assembly << "rjmp start\n\n";
      assembly << ".include \"macros.inc\" ; Biblioteca de contas\n\n";
      assembly << "start:\n";
      
      assembly.close();
      return true;
    }

    bool endCode(string filename = "main.asm"){
      ofstream assembly;
      assembly.open(filename, ios_base::app);

      assembly << "final_cod:\n";
		  assembly << "  rjmp final_cod\n\n";
      assembly << "tabela:\n";
      assembly << "  .db	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F\n\n";
      assembly << "pega_tabela:\n";
      assembly << "  push	R16\n";
      assembly << "   ldi		ZL, LOW(tabela<<1)\n";
      assembly << "   ldi		ZH, HIGH(tabela<<1)\n";
      assembly << "   add		ZL,R16\n";
      assembly << "   ldi		R16,0\n";
      assembly << "   adc		ZH,R16\n";
      assembly << "   lpm		R17,Z\n";
      assembly << "  pop R16\n";
      assembly << "  ret\n";
      assembly << "\n; --------------- CODE END ---------------\n";
      
      assembly.close();
      return true;
    }

    bool compile(vector<string> entries, string method, string filename = "main.asm"){
      //Open file
      ofstream assembly;
      assembly.open(filename, ios_base::app);
      
      //No need to treat numbers
      if(method == "Output"){
        string port = entries[2];
        string display = entries[1];
        string value = entries[0];
        string code = this->output(port,display,value);
        if (code == ""){
          cout << "Output operation is invalid! \n";
          return false;
        }
        assembly << code << "\n";
        return true;
      } else if(method == "Program"){
        cout << "Expected output = " << lastResult << "\n";
        assembly << ";Expected output = " << lastResult << "\n";
        assembly << "; --------------- PROGRAM END ---------------\n\n";
        return true;
      }

      //Check if operation is possible
      string n1 = entries[2];
      string n2 = entries[1];
      bool hasFloat = (n1.find('.') != string::npos) ? true : ((n2.find('.') != string::npos) ? true : false);
      //cout << "hasFloat: " << hasFloat << "\n";
      float num1 = n1=="R16" ? this->lastResult : stof(n1);
      float num2 = n2=="R16" ? this->lastResult : stof(n2);

      int isValid = this->isValidOperation(num1,num2,method,hasFloat);
      if(isValid != 0){
        cout << (isValid == 2 ? "Operation surpassed the threshold of 1.0 <= |result| <= 15.9! \n" : "Invalid use of float in power or remainder! \n");
        return false;
      }
      
      //Handle arithimetical operator     
      if(method == "+"){
        if(num1 < 0){                                                                 // n1           n2
          if(num2 < 0)              assembly << this->add(n1,n2,true) << "\n";        // neg          neg
          else if(-num1 < num2)     assembly << this->subtract(n2,n1,false) << "\n";  // neg          pos(maior)
          else                      assembly << this->subtract(n1,n2,true) << "\n";   // neg          pos(menor)
        } else if(num2 < 0){
          if(num1 < -num2)         assembly << this->subtract(n2,n1,true) << "\n";    // pos(menor)   neg
          else                     assembly << this->subtract(n1,n2,false) << "\n";   // pos(maior)   neg
        } else {
                                   assembly << this->add(n1,n2,false) << "\n";        // pos          pos
        }   
      } else if(method == "-"){
        if(num1 < 0){                                                                 // n1           n2
          if(num2 > 0)              assembly << this->add(n1,n2,true) << "\n";        // neg          pos
          else if(-num1 < -num2)    assembly << this->subtract(n2,n1,false) << "\n";  // neg(menor)   neg
          else                      assembly << this->subtract(n1,n2,true) << "\n";   // neg(maior)   neg
        } else if(num2 < 0){
                                    assembly << this->add(n1,n2,false) << "\n";       // pos          neg
        } else {
          if(num1 < num2)           assembly << this->subtract(n2,n1,true) << "\n";   // pos(menor)   pos
          else                      assembly << this->subtract(n1,n2,false) << "\n";  // pos(maior)   pos
        } 
      } else if(method == "*"){
        if((num1 < 0 && num2 < 0) || (num1 > 0 && num2 > 0)){                         // n1           n2
                                    assembly << this->multiply(n1,n2,false) << "\n";  // neg          neg
                                                                                      // pos          pos
        } else {
                                    assembly << this->multiply(n1,n2,true) << "\n";   // pos          neg
                                                                                      // neg          pos
        } 
      } else if(method == "/"){
        bool negativeSign = !((num1 < 0 && num2 < 0) || (num1 > 0 && num2 > 0));      // True if pos else False
        
        if (abs(num1) < abs(num2)) assembly << this->divide(n2,n1,negativeSign,cont) << "\n";   
        else assembly << this->divide(n1,n2,negativeSign,cont) << "\n";   
        
      } else if(method == "%"){
        bool negativeSign = !((num1 < 0 && num2 < 0) || (num1 > 0 && num2 > 0));      // True if pos else False
        
        if (abs(num1) < abs(num2)) assembly << this->remainder(n2,n1,negativeSign,cont) << "\n";   
        else assembly << this->remainder(n1,n2,negativeSign,cont) << "\n";

      } else if(method == "**"){                                                           // n1           n2
        if(num2 < 0){                                                                      // *            neg
          assembly.close();
          assembly.open(filename); //clear file
          assembly.close();
          cout << "Its not possible to calculate a number to the power of a negative number!\n";
          return false;
        } else if(num1 > 0){
                                    assembly << this->power(n1,n2,false,cont) << "\n";             // pos          pos
        } else {
          if (static_cast<int>(num2)%2 == 0) assembly << this->power(n1,n2,false,cont) << "\n";    // neg(par)     pos
          else                      assembly << this->power(n1,n2,true,cont) << "\n";              // neg(impar)   pos
        }
      } else {
        assembly.close();
        assembly.open(filename); //clear file
        assembly.close();
        cout << "Invalid syntax -> '" + method + "'\n";
        return false;
      }

      assembly.close();
      return true;
    }

  private:
    int cont = 0;
    float lastResult = -1;

    int isValidOperation(float x, float y, string method, bool hasFloat){
      float result;
      if(method == "+")
        result = x+y;
      else if(method == "-")
        result = x-y;
      else if(method == "*")
        result = x*y;
      else if(method == "/")
        result = x/y;
      else if(method == "%" && hasFloat == false)
        result = static_cast<int>(x)%static_cast<int>(y);
      else if(method == "**" && hasFloat == false)
        result = pow(x,y);
      else
        return 1;
      this->lastResult = result;
      //cout << "lastResult = " << result << "\n";
      return abs(result) <= 15.9 && abs(result) >= 1.0 ? 0 : 2;
    }

    string add(string n1, string n2, bool negative){
      if (n1.find('-') != string::npos) n1.erase(0,1);
      if (n2.find('-') != string::npos) n2.erase(0,1);
      string code = "";
     
      code += dealWithEntries(n1, n2); // Check if entries are numbers and convert them to IEE6
      code += "sum R26, R27, R16, R17\n";
      code += negative ? "LDI R25, 1\n" : "LDI R25, 0\n";
    
      return code;
    }
    
    string subtract(string n1, string n2, bool negative){
      if (n1.find('-') != string::npos) n1.erase(0,1);
      if (n2.find('-') != string::npos) n2.erase(0,1);
      string code = "";
     
      code += dealWithEntries(n1, n2); // Check if entries are numbers and convert them to IEE6
      code += "subtraction R26, R27, R16, R17\n";
      code += negative ? "LDI R25, 1\n" : "LDI R25, 0\n";
    
      return code;
    }
    
    string multiply(string n1, string n2, bool negative){
      if (n1.find('-') != string::npos) n1.erase(0,1);
      if (n2.find('-') != string::npos) n2.erase(0,1);
      string code = "";
     
      code += dealWithEntries(n1, n2); // Check if entries are numbers and convert them to IEE6
      code += "mult R26, R27, R16, R17\n";
      code += negative ? "LDI R25, 1\n" : "LDI R25, 0\n";
      
      return code;
    }
    
    string divide(string n1, string n2, bool negative, int cont){
      if (n1.find('-') != string::npos) n1.erase(0,1);
      if (n2.find('-') != string::npos) n2.erase(0,1);
      string code = "";
     
      code += dealWithEntries(n1, n2); // Check if entries are numbers and convert them to IEE6
      code += "div R26, R27, R16, R17\n";
      code += negative ? "LDI R25, 1\n" : "LDI R25, 0\n";
    
      return code;
    }
    
    string remainder(string n1, string n2, bool negative, int cont){
      if (n1.find('-') != string::npos) n1.erase(0,1);
      if (n2.find('-') != string::npos) n2.erase(0,1);
      string code = "";
     
      code += dealWithEntries(n1, n2); // Check if entries are numbers and convert them to IEE6
      code += "resto R26, R27, R16, R17\n";
      code += negative ? "LDI R25, 1\n" : "LDI R25, 0\n";
    
      return code;
    }
    
    string power(string n1, string n2, bool negative, int cont){
      if (n1.find('-') != string::npos) n1.erase(0,1);
      if (n2.find('-') != string::npos) n2.erase(0,1);
      string code = "";
     
      code += dealWithEntries(n1, n2); // Check if entries are numbers and convert them to IEE6
      code += "potencia R26, R27, R16, R17\n";
      code += negative ? "LDI R25, 1\n" : "LDI R25, 0\n";
    
      return code;
    }

    string output(string port, string displayMethod, string value){
      string code = "";
      code += "separa_inteiro_decimal R26, R16\n";
      
      if (port == "PD" && displayMethod == "Display"){
        code += "mostra_resultado_display\n";
      } else if (port == "PC" && (displayMethod == "Botao" || displayMethod == "Temporizador")){
        code += displayMethod == "Botao" ? "mostra_resultado_botao\n" : "mostra_resultado_temp\n";
      } else if (port == "PB" && (displayMethod == "Botao" || displayMethod == "Temporizador" || displayMethod == "Display")){
        code += displayMethod == "Botao" ? "mostra_resultado_botao\n" : (displayMethod=="Display" ? "mostra_resultado_display\n" : "mostra_resultado_temp\n");
      } else {
        return "";
      }     
      return code;
    }

    vector<string> split(string st){
      stringstream test(st);
      string segment;
      vector<string> seglist;
      
      while(getline(test, segment, '.'))
      {
         seglist.push_back(segment);
      }
      return seglist;
    }

    string dealWithEntries(string n1, string n2){
      // n1 is the biggest number and n2 is the smallest number but only if operation is not "potencia"
      string code = "";
      
      if (n1 == "R16"){
        code += "PUSH R16\nPUSH R26\n"; 
      } else {
        if (n2 == "R16"){
          code += "PUSH R16\nPUSH R26\n";
        }
        vector<string> parts = split(n1);
        string inteira = parts[0];
        string decimal = parts.size() > 1 ? parts[1] : "0";
        code += "LDI R16, ";
        code += inteira;
        code += "\n";
        code += "LDI R26, ";
        code += decimal; 
        code += "\n";
        code += "passa_pra_iee R16, R26\n";
        code += "PUSH R17\n";
        code += "PUSH R27\n";
      }

      if (n2 == "R16"){
        code += "POP R26\nPOP R16\n";
        code += "POP R27\nPOP R17\n";
      } else {
        vector<string> parts = split(n2);
        string inteira = parts[0];
        string decimal = parts.size() > 1 ? parts[1] : "0";
        code += "LDI R16, ";
        code += inteira;
        code += "\n";
        code += "LDI R26, ";
        code += decimal; 
        code += "\n";
        code += "passa_pra_iee R16, R26\n";
        code += "POP R26\nPOP R16\n";
      }

      return code;
    };
};

#endif
