#pragma once
#include "debug.h"
#include <vector>
#include <string>
#include "ast.h"
using namespace std;

enum cmd_type{
  enum_nop,
  enum_match_char_c,
  enum_match_char_any,
  enum_match_special_n,
  enum_match_range_from_c,
  enum_match_range_to_c,
  enum_move_to_prev_char,
  enum_naitve_func_n, 
  enum_loop_begin_n, 
  enum_loop_check_and_try_p,
  enum_loop_inc_n, 
  enum_try_p,
  enum_test_and_roll_back,
  enum_roll_back,
  enum_roll_back2,
  enum_jmp_p,
  enum_capture_begin_n,
  enum_capture_end_n,
  enum_success
};

string cmd_type_names(int cmd);
bool cmd_type_arg_is_uchar(int cmd);


class cmd_item{
public:
  int cmd;
  uint32_t arg1;
};


class cmd_item_builder{
public:
  cmd_item ci;
  size_t address;
  cmd_item_builder* argp;
  cmd_item_builder* next;
  cmd_item_builder(int cmd=0, uint32_t arg1=0){
    ci.cmd= cmd;
    ci.arg1= arg1;
    address= 0;
    argp=next=nullptr;
  }
  cmd_item_builder& operator>>(cmd_item_builder* _next){
      next= _next;
      return *_next;
  }
};

class cmd_item_builder_list{
public:
  cmd_item_builder_list(cmd_item_builder* b, cmd_item_builder* e):begin(b),end(e){}
  cmd_item_builder *begin,*end;
};


/**
 * insert il code into out
 * out.begin and out.end need to be set to hold the il code.
 * before call this func, expect there is *nothing* between out.begin and out.end .
 * @return added li code's begin and end
 */
void bulid_il_ast_node(ast_node* ast, cmd_item_builder* _prev, cmd_item_builder* _next);
