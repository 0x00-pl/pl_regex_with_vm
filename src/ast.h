#pragma once
#include <vector>
#include <iostream>
#include "str_convert.h"

using namespace std;

enum ast_char_special_type{
  newline =1 , //'\n'
  carriage, //'\r'
  form_feed, //'\f'
  vertical_tab, //'\v'
  tab, //'\t'
  alnum, //lowercase letters, uppercase letters, and digits
  alpha, //lowercase letters and uppercase letters
  blank, //space or tab 
  cntrl, //the file format escape characters
  digit, //digits
  graph, //lowercase letters, uppercase letters, digits, and punctuation
  lower, //lowercase letters
  print, //lowercase letters, uppercase letters, digits, punctuation, and space
  punct, //punctuation
  space, //space
  upper, //uppercase characters
  xdigit, //digits, 'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F'
  N_digit, //not digit
  N_space, //not space
  N_alnum,  //not alnum
  special_type_end
};


class ast_node{
public:
  virtual ~ast_node(){}
  virtual void print(ostream& s){
    s<<"ast_node()";
  }
};

class ast_node_char_any: public ast_node{
public:void print(ostream& s){
    s<<".";
  }
};
class ast_node_char_const: public ast_node{
public:
  uint32_t ch;
  ast_node_char_const(uint32_t _ch):ch(_ch){}
  void print(ostream& s){
    s<<(char)ch;
  }
};
class ast_node_char_range: public ast_node{
public:
  uint32_t min;
  uint32_t max;
  void print(ostream& s){
    s<<(char)min<<"-"<<(char)max;
  }
};
class ast_node_char_special: public ast_node{
public:
  int type;
  ast_node_char_special(int _type):type(_type){}
  void print(ostream& s){
    s<<"special("<<type<<")";
  }
};
class ast_node_char_backref: public ast_node{
public:
  size_t index;
  c_ustr name;
  void print(ostream& s){
    if(index!=0){
      s<<"\\"<<index;
    }else{ 
      s<<"\\<";
      name.print(s);
      s<<">";
    }
  }
};

class ast_node_seq: public ast_node{
public:
  vector<ast_node*> sub;
  ~ast_node_seq(){
    for(ast_node* i : sub) delete i;
  }
  void print(ostream& s){
    for(size_t i=0; i<sub.size(); i++){
      sub[i]->print(s);
    }
  }
};

class ast_node_capture: public ast_node{
public:
  vector<ast_node*> sub_branchs;
  size_t repeat_min, repeat_max;
  bool greedy;
  bool caret_mark;
  int group_index; // -1 is none
  c_ustr group_name; 
  
  ast_node_capture(){
    greedy=false;
    caret_mark=false;
    group_index=-1;
    repeat_min=repeat_max=1;
  }
  ~ast_node_capture(){
    for(ast_node* i : sub_branchs) delete i;
  }
  void print(ostream& s);
};


ast_node* ast_parse(c_ustr& str, size_t& pos);