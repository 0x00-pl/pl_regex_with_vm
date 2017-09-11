#pragma once
#include "debug.h"
#include <string>
#include <map>
#include <vector>
#include "vm.h"
using namespace std;



class pl_regex{
public:
  pl_regex(string s);
  bool match(string s);
  vector<vm_capture> captures;
  vm_state_static _compired;
};