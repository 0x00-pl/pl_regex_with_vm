#pragma once
#include <iostream>
#include <string>
using namespace std;

uint32_t convert_utf8_to_unicode(const char *&p_io, const char* p_end);
size_t convert_unicode_to_utf8(uint32_t u, char* c_out);

uint32_t uchar(const char * c);

class c_ustr{
public:
  uint32_t *base;
  size_t size;
  c_ustr(const char* s, const char* s_end);
  c_ustr(string s);
  c_ustr(uint32_t* _base, size_t _size);
  c_ustr(){ base=nullptr; size=0; }
  ~c_ustr(){ delete[] base; size=0; }
  bool is_null(){return base==nullptr;}
  uint32_t operator[](size_t p){return base[p];}
  void set(uint32_t* _base, size_t _size);
  void print(ostream& s){
    size_t cstr_size=0;
    for(size_t i=0; i<size; i++){
      cstr_size+= convert_unicode_to_utf8(base[i], nullptr);
    }
    if(cstr_size<1024){
      char cstr[cstr_size+1];
      cstr[cstr_size]='\0';
      char* p= cstr;
      for(size_t i=0; i<size; i++){
	p+= convert_unicode_to_utf8(base[i], p);
      }
      s<<cstr;
    }else{
      char* cstr= new char[cstr_size+1];
      cstr[cstr_size]='\0';
      char* p= cstr;
      for(size_t i=0; i<size; i++){
	p+= convert_unicode_to_utf8(base[i], p);
      }
      s<<cstr;
      delete[] cstr;
    }
  }
};

