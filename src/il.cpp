#include "il.h"
#include <typeinfo>
#include <limits>
using namespace std;

void set_next(cmd_item_builder* a, cmd_item_builder* b){ a->next=b; }
cmd_item_builder* cmd_alloc(int cmd, uint32_t arg1=0){
  return new cmd_item_builder(cmd, arg1);
}


void bulid_il_ast_node_char_any(ast_node_char_any* ast, cmd_item_builder* _prev, cmd_item_builder* _next){
  (void)ast;
  auto ma= cmd_alloc(cmd_type::enum_match_char_any);
  auto roll_back= cmd_alloc(cmd_type::enum_test_and_roll_back);
  *_prev>>ma>>roll_back>>_next;
}
void bulid_il_ast_node_char_const(ast_node_char_const* ast, cmd_item_builder* _prev, cmd_item_builder* _next){
  auto mc= cmd_alloc(cmd_type::enum_match_char_c, ast->ch);
  auto roll_back= cmd_alloc(cmd_type::enum_test_and_roll_back);
  *_prev>>mc>>roll_back>>_next;
}
void bulid_il_ast_node_char_range(ast_node_char_range* ast, cmd_item_builder* _prev, cmd_item_builder* _next){
  auto mmin= cmd_alloc(cmd_type::enum_match_range_from_c, ast->min);
  auto mvpc= cmd_alloc(cmd_type::enum_move_to_prev_char);
  auto mmax= cmd_alloc(cmd_type::enum_match_range_to_c, ast->max);
  auto roll_back= cmd_alloc(cmd_type::enum_test_and_roll_back);
  *_prev>>mmin>>mvpc>>mmax>>roll_back>>_next;
}
void bulid_il_ast_node_char_special(ast_node_char_special* ast, cmd_item_builder* _prev, cmd_item_builder* _next){
  auto spe= cmd_alloc(cmd_type::enum_match_special_n, ast->type);
  auto roll_back= cmd_alloc(cmd_type::enum_test_and_roll_back);
  *_prev>>spe>>roll_back>>_next;
}
void bulid_il_ast_node_char_backref(ast_node_char_backref* ast, cmd_item_builder* _prev, cmd_item_builder* _next){
  (void)ast; (void)_prev; (void)_next;
  throw "undefined TODO bulid_il_ast_node_char_backref()";
}
void bulid_il_ast_node_seq(ast_node_seq* ast, cmd_item_builder* _prev, cmd_item_builder* _next){
  vector<ast_node*>& seq_nodes= ast->sub;
  _prev->next= _next;
  for(size_t i=0; i<seq_nodes.size(); i++){
    size_t ri= seq_nodes.size()-i-1;
    bulid_il_ast_node(seq_nodes[ri], _prev, _prev->next);
  }
}
cmd_item_builder* bulid_il_ast_node_capture_loop(ast_node_capture* ast, cmd_item_builder* _prev, cmd_item_builder* _next){
  if(ast->repeat_min==1 && ast->repeat_max==1){ 
    *_prev>>_next;
    return _prev; 
  }
  auto _loop_begin= cmd_alloc(cmd_type::enum_loop_begin_n, ast->repeat_index);
  auto _escape_point= cmd_alloc(cmd_type::enum_loop_check_and_try_p);
  auto _loop_inc= cmd_alloc(cmd_type::enum_loop_inc_n, ast->repeat_index);
  auto _loop_jmp= cmd_alloc(cmd_type::enum_jmp_p);
  *_prev>> _loop_begin
	>> _escape_point
	>> _loop_inc
	>> _loop_jmp
	>> _next;
  _escape_point->argp= _next;
  _loop_jmp->argp= _escape_point;
  return _escape_point;
  /*
  if(ast->repeat_max==std::numeric_limits<size_t>::max()){
    if(ast->repeat_min==0){
      // b.*e 
      _jmp_out->next= _next;
      _jmp->next= _check_point;
      TEMPDEF_LINK_5(_prev, _check_point, _jmp_out, _jmp, _next)
      return _jmp_out;
    }else{
      // skip _loop_max etc.
      TEMPDEF_LINK_2(_loop_begin, _loop_min)
    }
  }
    
  if(!ast->greedy){
    TEMPDEF_CMD(_jmp_backup, cmd_type::enum_jmp_p, 0)
    _jmp->argp= _nop;
    _jmp_backup->argp= _next;
    TEMPDEF_LINK_6(_loop_min, _check_point, _jmp, _jmp_backup, _nop, _loop_end_j)
    **
     * _loop_min +3
     * _check_point
     * _jmp -> _nop
     * _jmp_backup -> _next
     * _nop      <--
     *
    return _nop;
  }
  return _jmp;
  */
}
void bulid_il_ast_node__link(vector<ast_node*>& nodes, cmd_item_builder* _prev, cmd_item_builder* _next){
  for(size_t i=0; i<nodes.size(); i++){
    size_t ri= nodes.size()-i-1;
    bulid_il_ast_node(nodes[ri], _prev, _next);
    _next= _prev->next;
  }
}
void bulid_il_ast_node_capture_branch__chars_reverse(ast_node_capture* ast, int& index, vector<ast_node*>& ret){
  ret.clear();
  while(index>=0
    && (typeid(*ast->sub_branchs[index])==typeid(ast_node_char_any)
      || typeid(*ast->sub_branchs[index])==typeid(ast_node_char_const)
      || typeid(*ast->sub_branchs[index])==typeid(ast_node_char_special)
      || typeid(*ast->sub_branchs[index])==typeid(ast_node_char_range)
    ))
  {
    ret.push_back(ast->sub_branchs[index]);
    index--; //reverse
  }
}
void bulid_il_ast_node_capture_branch(ast_node_capture* ast, cmd_item_builder* _prev, cmd_item_builder* _next){
  if(ast->sub_branchs.size()==1){
    if(typeid(*ast->sub_branchs[0])==typeid(ast_node_seq)){
      bulid_il_ast_node__link(dynamic_cast<ast_node_seq*>(ast->sub_branchs[0])->sub, _prev, _next);
    }else if(typeid(*ast->sub_branchs[0])==typeid(ast_node_char_any)
      || typeid(*ast->sub_branchs[0])==typeid(ast_node_char_const)
      || typeid(*ast->sub_branchs[0])==typeid(ast_node_char_special)
      || typeid(*ast->sub_branchs[0])==typeid(ast_node_char_range)){
	bulid_il_ast_node(ast->sub_branchs[0], _prev, _next);
      }else{
	throw "debug unknow";
      }
    return;
  }
  vector<ast_node*> temp_vec_ast_node;
  cmd_item_builder* branch_entry;
  cmd_item_builder* branch_entry_next;
  cmd_item_builder* body_entry;
  cmd_item_builder* body_entry_next;
  
  if(ast->caret_mark){
    auto _ma= cmd_alloc(cmd_type::enum_match_char_any);
    auto _jmp_out= cmd_alloc(cmd_type::enum_jmp_p);
    _jmp_out->argp= _next;
    *_prev>>_ma>>_jmp_out>>_next;
    
    branch_entry= _prev; branch_entry_next= _ma;
    body_entry= _jmp_out; body_entry_next= _next;
  }else{
    auto _roll_back= cmd_alloc(cmd_type::enum_roll_back);
    *_prev>>_roll_back>>_next;
    
    branch_entry= _prev; branch_entry_next= _roll_back;
    body_entry= _roll_back; body_entry_next= _next;
  }
  
  for(int i=ast->sub_branchs.size()-1; i>=0 ;){
    if(ast->caret_mark){
      auto _roll_back2= cmd_alloc(cmd_type::enum_roll_back2);
      *body_entry>>_roll_back2>>body_entry_next;
      body_entry_next= _roll_back2;
    }else{
      auto _jmp_out= cmd_alloc(cmd_type::enum_jmp_p);
      *body_entry>>_jmp_out>>body_entry_next;
      _jmp_out->argp= _next;
      body_entry_next= _jmp_out;
    }
    
    bulid_il_ast_node_capture_branch__chars_reverse(ast, i, temp_vec_ast_node); // i changed.
    if(temp_vec_ast_node.empty()){
      if(typeid(*ast->sub_branchs[i])==typeid(ast_node_seq)){
	bulid_il_ast_node__link(dynamic_cast<ast_node_seq*>(ast->sub_branchs[i])->sub, body_entry, body_entry_next);
	i--;
      }else{
	throw "debug unknow";
      }
    }else{
      bulid_il_ast_node__link(temp_vec_ast_node, body_entry, body_entry_next);
    }
    body_entry_next= body_entry->next;
    
    auto _jmp_in_branch= cmd_alloc(cmd_type::enum_try_p);
    _jmp_in_branch->argp= body_entry;
    *branch_entry>>_jmp_in_branch>>branch_entry_next;
    branch_entry_next= branch_entry->next;
  }
}
void bulid_il_ast_node_capture(ast_node_capture* ast, cmd_item_builder* _prev, cmd_item_builder* _next){
  /**
   * loop(capture(branchs))
   */  
  cmd_item_builder* body_entry= bulid_il_ast_node_capture_loop(ast, _prev, _next);
  cmd_item_builder* body_entry_next= body_entry->next;
  
  if(ast->group_index!=-1){
    auto capture_beg= cmd_alloc(cmd_type::enum_capture_begin_n, ast->group_index);
    auto capture_end= cmd_alloc(cmd_type::enum_capture_end_n, ast->group_index);
    *body_entry>>capture_beg>>capture_end>>body_entry_next;
    body_entry= capture_beg;
  }
  
  bulid_il_ast_node_capture_branch(ast, body_entry, body_entry->next);
}

void bulid_il_ast_node(ast_node* ast, cmd_item_builder* _prev, cmd_item_builder* _next){

#define TEMPDEF_IF_CAST_CALL_ELSE(_type, ptr, ...)  \
  if(typeid( * ptr )==typeid( _type )){ \
    bulid_il_ ## _type (dynamic_cast<_type*>(ptr), __VA_ARGS__ );\
  }else
    
  TEMPDEF_IF_CAST_CALL_ELSE(ast_node_char_any, ast, _prev, _next)     
  TEMPDEF_IF_CAST_CALL_ELSE(ast_node_char_const, ast, _prev, _next)   
  TEMPDEF_IF_CAST_CALL_ELSE(ast_node_char_range, ast, _prev, _next)   
  TEMPDEF_IF_CAST_CALL_ELSE(ast_node_char_special, ast, _prev, _next) 
  TEMPDEF_IF_CAST_CALL_ELSE(ast_node_char_backref, ast, _prev, _next) 
  TEMPDEF_IF_CAST_CALL_ELSE(ast_node_seq, ast, _prev, _next)   
  TEMPDEF_IF_CAST_CALL_ELSE(ast_node_capture, ast, _prev, _next)        
  {
    throw "debug unknow";
  }

#undef TEMPDEF_IF_CAST_CALL_ELSE
}

string cmd_type_names(int cmd){
  switch(cmd){
    case enum_nop                  : return "nop                 ";
    case enum_match_char_c         : return "match_char_c        ";
    case enum_match_char_any       : return "match_char_any      ";
    case enum_match_special_n      : return "match_special_n     ";
    case enum_match_range_from_c   : return "match_range_from_c  ";
    case enum_match_range_to_c     : return "match_range_to_c    ";
    case enum_move_to_prev_char    : return "move_to_prev_char   ";
    case enum_naitve_func_n        : return "naitve_func_n       ";
    case enum_loop_begin_n         : return "loop_begin_n        ";
    case enum_loop_check_and_try_p : return "loop_check_and_try_p";
    case enum_loop_inc_n           : return "loop_inc_n          ";
    case enum_try_p                : return "try_p               ";
    case enum_test_and_roll_back   : return "test_and_roll_back  ";
    case enum_roll_back            : return "roll_back           ";
    case enum_roll_back2           : return "roll_back2          ";
    case enum_jmp_p                : return "jmp_p               ";
    case enum_capture_begin_n      : return "capture_begin_n     ";
    case enum_capture_end_n        : return "capture_end_n       ";
    case enum_success              : return "success             ";
    default                        : return "unknow                   ";
  }
  return "unknow                   ";
}
bool cmd_type_arg_is_uchar(int cmd){
  switch(cmd){
    case enum_nop                  : return false;
    case enum_match_char_c         : return true;
    case enum_match_char_any       : return false;
    case enum_match_special_n      : return true;
    case enum_match_range_from_c   : return true;
    case enum_match_range_to_c     : return true;
    case enum_move_to_prev_char    : return false;
    case enum_naitve_func_n        : return false;
    case enum_loop_begin_n         : return false;
    case enum_loop_check_and_try_p : return false;
    case enum_loop_inc_n           : return false;
    case enum_try_p                : return false;
    case enum_test_and_roll_back   : return false;
    case enum_roll_back            : return false;
    case enum_roll_back2           : return false;
    case enum_jmp_p                : return false;
    case enum_capture_begin_n      : return false;
    case enum_capture_end_n        : return false;
    case enum_success              : return false;
    default                        : return false;
  }
  return false;
}