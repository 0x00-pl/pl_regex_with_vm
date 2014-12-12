#include "str_convert.h"
#include <exception>
using namespace std;


uint32_t convert_utf8_to_unicode(const char *&p_io, const char* p_end){
  uint32_t mask_l1= 0xc0;
  uint32_t mask_l2= 0xe0;
  uint32_t mask_l3= 0xf0;
  uint32_t mask_l4= 0xf8;
  uint32_t mask_l5= 0xfc;
  uint32_t mask_6= (1<<6)-1;
  uint32_t ret=0;
  if(p_end-p_io<6){
    if((p_io[0]&mask_l5)==mask_l5 && p_end-p_io<6) throw "convert_utf8 fail: not enough length.";
    if((p_io[0]&mask_l4)==mask_l4 && p_end-p_io<5) throw "convert_utf8 fail: not enough length.";
    if((p_io[0]&mask_l3)==mask_l3 && p_end-p_io<4) throw "convert_utf8 fail: not enough length.";
    if((p_io[0]&mask_l2)==mask_l2 && p_end-p_io<3) throw "convert_utf8 fail: not enough length.";
    if((p_io[0]&mask_l1)==mask_l1 && p_end-p_io<2) throw "convert_utf8 fail: not enough length.";
    if(p_end-p_io<1) throw "convert_utf8 fail: not enough length.";
  }
  if(!(p_io[0]&0x80)){
    ret=p_io[0];
  }else if((p_io[0]&mask_l1)==mask_l1){
    ret= p_io[0]&((1<<5)-1);
    ret= (ret<<6) | ((++p_io)[0]&mask_6);
  }else if((p_io[0]&mask_l2)==mask_l2){
    ret= p_io[0]&((1<<4)-1);
    ret= (ret<<6) | ((++p_io)[0]&mask_6);
    ret= (ret<<6) | ((++p_io)[0]&mask_6);
  }else if((p_io[0]&mask_l3)==mask_l3){
    ret= p_io[0]&((1<<3)-1);
    ret= (ret<<6) | ((++p_io)[0]&mask_6);
    ret= (ret<<6) | ((++p_io)[0]&mask_6);
    ret= (ret<<6) | ((++p_io)[0]&mask_6);
  }else if((p_io[0]&mask_l4)==mask_l4){
    ret= p_io[0]&((1<<2)-1);
    ret= (ret<<6) | ((++p_io)[0]&mask_6);
    ret= (ret<<6) | ((++p_io)[0]&mask_6);
    ret= (ret<<6) | ((++p_io)[0]&mask_6);
    ret= (ret<<6) | ((++p_io)[0]&mask_6);
  }else if((p_io[0]&mask_l5)==mask_l5){
    ret= p_io[0]&((1<<1)-1);
    ret= (ret<<6) | ((++p_io)[0]&mask_6);
    ret= (ret<<6) | ((++p_io)[0]&mask_6);
    ret= (ret<<6) | ((++p_io)[0]&mask_6);
    ret= (ret<<6) | ((++p_io)[0]&mask_6);
    ret= (ret<<6) | ((++p_io)[0]&mask_6);
  }
  ++p_io;
  return ret;
}

/** return size of utf8 
 */
size_t convert_unicode_to_utf8(uint32_t u, char* c_out){
  size_t ret=1;
  if(u>0x0000007f) ret++;
  if(u>0x000007ff) ret++;
  if(u>0x0000ffff) ret++;
  if(u>0x001fffff) ret++;
  if(u>0x03ffffff) ret++;
  if(c_out!=nullptr){
    uint32_t mask_tail=0x2;
    uint32_t mask_6= (1<<6)-1;
    switch(ret){
      case 6:
	c_out[5]= (mask_tail<<6) | (u&mask_6);
	u=u>>6;
      case 5:
	c_out[4]= (mask_tail<<6) | (u&mask_6);
	u=u>>6;
      case 4:
	c_out[3]= (mask_tail<<6) | (u&mask_6);
	u=u>>6;
      case 3:
	c_out[2]= (mask_tail<<6) | (u&mask_6);
	u=u>>6;
      case 2:
	c_out[1]= (mask_tail<<6) | (u&mask_6);
	u=u>>6;
    }
    uint32_t mask_l1= 0xc0;
    uint32_t mask_l2= 0xe0;
    uint32_t mask_l3= 0xf0;
    uint32_t mask_l4= 0xf8;
    uint32_t mask_l5= 0xfc;
    switch(ret){
      case 6:
	c_out[0]= (u&((1<<1)-1)) | mask_l5; break;
      case 5:
	c_out[0]= (u&((1<<2)-1)) | mask_l4; break;
      case 4:
	c_out[0]= (u&((1<<3)-1)) | mask_l3; break;
      case 3:
	c_out[0]= (u&((1<<4)-1)) | mask_l2; break;
      case 2:
	c_out[0]= (u&((1<<5)-1)) | mask_l1; break;
      case 1:
	c_out[0]= u&0xff; break;
    }
  }
  return ret;
}

uint32_t uchar(const char * c){
  size_t len=0;
  while(c[len]!='\0') len++;
  return convert_utf8_to_unicode(c,c+len);
}

c_ustr::c_ustr(const char* s, const char* s_end){
  const char *s_for_size= s;
  size=0;
  while(s_for_size<s_end){
    convert_utf8_to_unicode(s_for_size,s_end);
    size++;
  }
  base= new uint32_t[size];
  for(size_t i=0; i<size; i++){
    base[i]=convert_utf8_to_unicode(s,s_end);
  }
}
c_ustr::c_ustr(uint32_t* _base, size_t _size){
  
  base= new uint32_t[_size];
  size= _size;
  for(size_t i=0; i<_size; i++)
    base[i]= _base[i];
}

c_ustr::c_ustr(string s):c_ustr(s.c_str(),s.c_str()+s.size()){}

void c_ustr::set(uint32_t* _base, size_t _size){
  delete[] base;
  
  base= new uint32_t[_size];
  size= _size;
  for(size_t i=0; i<_size; i++)
    base[i]= _base[i];
}


