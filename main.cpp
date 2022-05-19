#include <iostream>
#include <regex>
#include <string>
#include <stack>
#include <algorithm>

#include "Helper.cpp"

using namespace std;

/*
  FALTANDO:
    - Abrir arquivo com inputs;
*/

vector<string> split(string const &input);
bool resolve(stack<string> &stack, Helper *helper);

int main() {
  Helper *helper = new Helper();
  ifstream file;
  string query;

  //query = "(Program (Output PA Botao (3 (4 (1.8 1.2 +) *) +)))";
  helper->startCode();
  
  //Reads file 
  file.open ("options.txt");
  while(getline(file, query)){
    
    //Assure RPN format
    cout << "Analysing query: " << query << "\n";
    if (!regex_match (query, regex("^\\(.* .* .*\\)$"))){
      cout <<"Error: Invalid syntax -> '" << query << "'\n";
      break;
    }

    // Resolve 
    vector<string> pieces = split(query);
    stack<string> stack;
    for(string str : pieces){
      stack.push(str);
      if(str == ")"){
        if(!resolve(stack, helper)){
          cout << "Something happened :c \nCompilation aborted!\n";
          return 1;
        }
        stack.push("R16");
      }
    }
  }
 
  helper->endCode();
  cout << "Compilation successfuly completed!\n";
  return 0;
}

vector<string> split(string const &input) { 
  vector<string> ret;
  string temp = "";
  for (char chr : input) {
    if(chr == ' '){
      if(temp != ""){
        ret.push_back(temp);
        temp = "";
      } else
        continue;
    }
    else if (chr == '(' || chr == ')'){
      if(temp != ""){
        ret.push_back(temp);
        temp = "";
      }
      string aux;
      aux+=chr;
      ret.push_back(aux);
    }
    else {
      temp+=chr;
    }     
  }
  return ret;
}

bool resolve(stack<string> &stack, Helper *helper){
  string top, method = "";
  vector<string> values;
  vector<string> methods = {"+","-","*","/","%","**","Output","Program"};
  
  while (top != "(") {
    top = stack.top();
    values.push_back(top);
    if(*find(methods.begin(),methods.end(),top) == top)
      method = top;
    stack.pop();
  }

  vector<string> temp(values.begin(), values.end()); 
  reverse(temp.begin(), temp.end());
  for(auto str : temp){
    cout << str << " ";
  }
  cout << "\n";

  if(method != ""){
    vector<string> copy(values.begin()+1, values.begin()+values.size()-1);
    if(! helper->compile(copy, method))
      return false;
  }   
  else{
    cout << "No method found :( ! Stopping...\n";
    return false;
  }
  
  return true;
}