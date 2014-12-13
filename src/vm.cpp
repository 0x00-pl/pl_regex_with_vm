#include "vm.h"
#include "str_convert.h"
using namespace std;


bool lowercase_letters(uint32_t ch){
  return 'a'<=ch && ch<='z';
}
bool uppercase_letters(uint32_t ch){
  return 'A'<=ch && ch<='Z';
}
bool digits(uint32_t ch){
  return '0'<=ch && ch<='9';
}

bool match_special(uint32_t type, uint32_t ch){
  switch(type){
    case newline:
      return ch=='\n';
    case carriage:
      return ch=='\r';
    case form_feed:
      return ch=='\f';
    case vertical_tab:
      return ch=='\v';
    case tab:
      return ch=='\t';
    case space:
      return ch==' ';
    case N_space:
      return ch!=' ';
    case blank:
      return ch=='\t' || ch==' ';
    case lower:
      return lowercase_letters(ch);
    case upper:
      return uppercase_letters(ch);
    case alpha:
      return lowercase_letters(ch) || uppercase_letters(ch);
    case digit:
      return digits(ch);
    case N_digit:
      return !digits(ch);
    case xdigit:
      return digits(ch) || ('a'<=ch && ch<='f') || ('A'<=ch && ch<='F'); 
    case alnum:
      return lowercase_letters(ch) || uppercase_letters(ch) || digits(ch);
    case N_alnum:
      return !(lowercase_letters(ch) || uppercase_letters(ch) || digits(ch));
    case cntrl: //the file format escape characters
    case graph: //lowercase letters, uppercase letters, digits, and punctuation
    case print: //lowercase letters, uppercase letters, digits, punctuation, and space
    case punct: //punctuation
    case special_type_end:
    default:
      throw "debug unknow";
  }
  return false;
}

void run_step(vm_state& vm){
  size_t& next_pc= vm._states.back().pc;
  cmd_item& cur_cmd_item= vm._static.cmds[next_pc];
  size_t& cur_ptext= vm._states.back().ptext;
  next_pc++;
  
  switch(cur_cmd_item.cmd){
    case enum_nop: break;
    case enum_match_char_any:
      if(cur_ptext>=vm.text.size){ vm.rollback(); }
      cur_ptext++;
      break;
    case enum_match_char_c:
      if(cur_ptext>=vm.text.size){ vm.rollback(); }
      if(vm.text[cur_ptext]!=cur_cmd_item.arg1){
	vm.char_match_fail= true;
      }
      cur_ptext++;
      break;
    case enum_match_special_n:
      if(cur_ptext>=vm.text.size){ vm.rollback(); }
      if(!match_special(cur_cmd_item.arg1, vm.text[cur_ptext])){
	vm.char_match_fail= true;
      }
      cur_ptext++;
      break;
    case enum_match_range_from_c:
      if(cur_ptext>=vm.text.size){ vm.rollback(); }
      if(!(vm.text[cur_ptext]>=cur_cmd_item.arg1)){
	vm.char_match_fail= true;
      }
      cur_ptext++;
      break;
    case enum_match_range_to_c:
      if(cur_ptext>=vm.text.size){ vm.rollback(); }
      if(!(vm.text[cur_ptext]<=cur_cmd_item.arg1)){
	vm.char_match_fail= true;
      }
      cur_ptext++;
      break;
    case enum_move_to_prev_char:
      if(cur_ptext==0) throw "debug unknow, go prev of begin of string.";
      cur_ptext--;
      break;
    case enum_naitve_func_n:
      vm._static.native_func[cur_cmd_item.arg1](vm);
      break;
    case enum_loop_begin_n:
      vm._states.back().loop_num[cur_cmd_item.arg1]=0;
      vm.cur_loop= cur_cmd_item.arg1;
      break;
    case enum_loop_inc_n:
      vm._states.back().loop_num[cur_cmd_item.arg1]++;
      vm.cur_loop= cur_cmd_item.arg1;
      break;
    case enum_jmp_p:
      vm._states.back().pc= cur_cmd_item.arg1;
      break;
    case enum_loop_check_and_try_p:
      if(vm._states.back().loop_num[vm.cur_loop] < vm._static.loop_min[vm.cur_loop]){
	break;
      }
      if(vm._states.back().loop_num[vm.cur_loop] > vm._static.loop_max[vm.cur_loop]){
	vm._states.back().pc= cur_cmd_item.arg1;
	break;
      }
      if(vm._static.loop_greedy[vm.cur_loop]){
	vm.checkpoint(cur_cmd_item.arg1);
      }else{
	vm.checkpoint(next_pc);
	vm._states.back().pc= cur_cmd_item.arg1;
      }
      break;
    case enum_try_p:
      vm.checkpoint(next_pc);
      vm._states.back().pc= cur_cmd_item.arg1;
      break;
    case enum_test_and_roll_back:
      if(vm.char_match_fail) vm.rollback();
      vm.char_match_fail= false;
      break;
    case enum_roll_back:
      vm.rollback();
      break;
    case enum_roll_back2:
      vm.rollback();
      vm.rollback();
      break;
    case enum_capture_begin_n:
      vm.capture_push(cur_cmd_item.arg1, cur_ptext, vm_capture::_in);
      break;
    case enum_capture_end_n:
      vm.capture_push(cur_cmd_item.arg1, cur_ptext, vm_capture::_out);
      break;
    case enum_success:
      vm.rollback();
      break;
    default:
      throw "debug unknow";
  }
}

/*
bool run_vm(vm_state_static& _static, c_ustr& text, vector<vm_capture>& captures){
  vm_state _vm(_static, text, captures);
  while(!_vm._states.empty()){
    if(_vm._states.back().ptext >= text.size){
      if(_vm._static.cmds[_vm._states.back().pc].cmd==cmd_type::enum_success){
	return true;
      }
      run_step(_vm);
    }
  }
  return false;
}*/
void vm_state::rollback(){
#ifdef PL_DEBUG
  cout<<"rollback from ";
  _states.back().print(cout);
#endif
  _states.pop_back();
#ifdef PL_DEBUG
  cout<<" to ";
  if(_states.empty()){
    cout<<"(empty)";
  }else{
    _states.back().print(cout);
  }
  cout<<endl;
#endif
}

void vm_state::checkpoint(uint32_t pc_when_roll_back){
  _states.push_back(_states.back());
  _states.back().pc= pc_when_roll_back;
}

void vm_state::capture_push(uint32_t capture_id, size_t p_text, bool _in_or_out){
  size_t& next_capture_pos= _states.back().next_capture_pos;
  if(captures.size()>next_capture_pos){
    captures[next_capture_pos]= vm_capture(capture_id, p_text, _in_or_out);
  }else{
    captures.push_back(vm_capture(capture_id, p_text, _in_or_out));
  }
  next_capture_pos++;
}

bool vm_state::run(){
  while(!_states.empty()){
    if(_states.back().ptext >= text.size){
      if(_static.cmds[_states.back().pc].cmd==cmd_type::enum_success){
	captures.resize(_states.back().next_capture_pos);
	return true;
      }
    }
    run_step(*this);
  }
  return false;
}

