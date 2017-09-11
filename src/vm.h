#pragma once
#include "debug.h"
#include "il.h"
#include <vector>
#include <map>
#include <functional>
using namespace std;

class vm_state;

class vm_state_static{
public:
  size_t capture_pool_size;
  size_t loop_pool_size;
  vector<cmd_item> cmds;
  vector<size_t> loop_min;
  vector<size_t> loop_max;
  vector<bool> loop_greedy;
  map<size_t, c_ustr> capture_id_name;
  vector<function<void(vm_state&)> > native_func;
};

class vm_state_running_state{
public:
  size_t pc;
  size_t ptext;
  size_t next_capture_pos;
  vector<size_t> loop_num;
  ostream& print(ostream& _out){
    _out<<"(pc="<<pc
    <<", ptext="<<ptext
    <<", pcapture="<<next_capture_pos
    <<")";
    return _out;
  }
};

class vm_capture{
public:
  uint32_t capture_id;
  size_t p_text;
  bool _in_or_out;
  
  static const bool _in=true;
  static const bool _out=false;
  vm_capture()=default;
  vm_capture(uint32_t _capture_id, size_t _p_text, bool __in_or_out)
  :capture_id(_capture_id)
  ,p_text(_p_text)
  ,_in_or_out(__in_or_out){}
};

class vm_state{
public:
  vm_state_static& _static;
  c_ustr& text;
  vector<vm_capture>& captures;
  vector<vm_state_running_state> _states;
  size_t cur_loop;
  bool char_match_fail;
  
  vm_state(vm_state_static& __static, c_ustr& __text, vector<vm_capture>& _captures)
  : _static(__static), text(__text), captures(_captures){
    char_match_fail= false;
    captures.clear();
    _states.push_back(vm_state_running_state());
    _states[0].pc=0;
    _states[0].ptext=0;
    _states[0].next_capture_pos=0;
    _states[0].loop_num= vector<size_t>(__static.loop_pool_size);
  }
  
  bool run();
  void checkpoint(uint32_t pc_when_roll_back);
  void rollback();
  void capture_push(uint32_t capture_id, size_t p_text, bool _in_or_out);
};

