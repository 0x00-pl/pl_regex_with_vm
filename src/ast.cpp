#include "ast.h"
#include <limits>
using namespace std;


class ast_parse_global_state{
public:
  size_t capture_index;
  bool at_square_bracket;
};

uint32_t read_digits(c_ustr& str, size_t& pos, int maxsize){
    uint32_t cc=0;
    while(maxsize==-1 || maxsize-->0){
      if(pos>=str.size) return 0;
      uint32_t cur_dig;
      cc= cc*10;
      if('0'<=str[pos] && str[pos]<='9'){ cur_dig= str[pos]-'0'; }
      else {break;}
      cc+= cur_dig;
      pos++;
    }
    return cc;
}
uint32_t read_xdigits(c_ustr& str, size_t& pos, int maxsize){
    uint32_t cc=0;
    while(maxsize==-1 || maxsize-->0){
      if(pos>=str.size) return 0;
      uint32_t cur_dig;
      cc= cc*16;
      if('0'<=str[pos] && str[pos]<='9'){ cur_dig= str[pos]-'0'; }
      else if('A'<=str[pos] && str[pos]<='F'){ cur_dig= str[pos]-'A'; }
      else if('a'<=str[pos] && str[pos]<='f'){ cur_dig= str[pos]-'a'; }
      else {break;}
      cc+= cur_dig;
      pos++;
    }
    return cc;
}

ast_node* ast_parse_char(c_ustr& str, size_t& pos, ast_parse_global_state& gst){
  if(pos>=str.size) return nullptr;
  if(str[pos]=='.'){
    pos++;
    return new ast_node_char_any;
  }
  if(gst.at_square_bracket 
    && pos+3<str.size
    && str[pos+1]=='-')
  {
    ast_node_char_range* ret= new ast_node_char_range;
    ret->min=str[pos];
    ret->max=str[pos+2];
    pos+=3;
    return ret;
  }
  if(str[pos]=='\\' && pos+1<str.size){
    pos++;
    if(str[pos]=='c'){
      // <ctrl->
      pos++;
      uint32_t cc;
      if('a'<=str[pos] && str[pos]<='z'){ cc= str[pos]-'a'; }
      else if('A'<=str[pos] && str[pos]<='Z'){ cc= str[pos]-'a'; }
      else if(str[pos]=='['){ cc=27; }
      else if(str[pos]=='\\'){ cc=28; }
      else if(str[pos]==']'){ cc=29; }
      else if(str[pos]=='^'){ cc=30; }
      else if(str[pos]=='_'){ cc=31; }
      
      pos++;
      return new ast_node_char_const({cc});
      
    }else if(str[pos]=='u' && pos+4<str.size){// \u....
      pos++;
      uint32_t cc=read_xdigits(str,pos,4);
      return new ast_node_char_const({cc});
    }else if(str[pos]=='x' && pos+2<str.size){// \x..
      pos++;
      uint32_t cc=read_xdigits(str,pos,2);
      return new ast_node_char_const({cc});
    }
    if('0'<=str[pos] && str[pos]<='9'){
      uint32_t n= read_digits(str,pos,1);
      ast_node_char_backref* ret= new ast_node_char_backref;
      ret->index=n;
      return ret;
    }
    int special_type;
    switch(str[pos]){
      case 'n': special_type= ast_char_special_type::newline; break;
      case 'r': special_type= ast_char_special_type::carriage; break;
      case 'f': special_type= ast_char_special_type::form_feed; break;
      case 'v': special_type= ast_char_special_type::vertical_tab; break;
      case 't': special_type= ast_char_special_type::tab; break;
      case 'd': special_type= ast_char_special_type::digit; break;
      case 'D': special_type= ast_char_special_type::N_digit; break;
      case 's': special_type= ast_char_special_type::space; break;
      case 'S': special_type= ast_char_special_type::N_space; break;
      case 'w': special_type= ast_char_special_type::alnum; break;
      case 'W': special_type= ast_char_special_type::N_alnum; break;
      default:
	return new ast_node_char_const({str[pos++]});
    }
    pos++;
    return new ast_node_char_special(special_type);
  }
  return new ast_node_char_const({str[pos++]});
}

void ast_parse_repeat(c_ustr& str, size_t& pos, size_t& min, size_t& max, bool& greedy){
  switch(str[pos]){
    case '?':
      pos++;
      min=0; max=1;
      break;
    case '+':
      pos++;
      min=1; max= std::numeric_limits<size_t>::max();
      break;
    case '*':
      pos++;
      min=0; max= std::numeric_limits<size_t>::max();
      break;
    case '{':
      pos++;
      if(str[pos]==','){
	min=0;
      }else{
        min= read_digits(str,pos,-1);
      }
      
      if(str[pos]==','){
	pos++;
	if(str[pos]=='}'){
	  max= std::numeric_limits<size_t>::max();
	}else{
	  max= read_digits(str,pos,-1);
	}
      }else{
	max=min;
      }
      pos++;
  }
  if(pos<str.size){
    greedy= str[pos]=='?';
    if(greedy) pos++;
  }
}

ast_node* ast_parse_char_may_repeat(c_ustr& str, size_t& pos, ast_parse_global_state& gst){
  ast_node* ch= ast_parse_char(str, pos, gst);
  if(pos<str.size &&(
    str[pos]=='?' || str[pos]=='+' || str[pos]=='*' || str[pos]=='{'
  )){
    ast_node_capture* ch_repeat= new ast_node_capture;
    ch_repeat->sub_branchs.push_back(ch);
    ast_parse_repeat(str,pos, ch_repeat->repeat_min, ch_repeat->repeat_max, ch_repeat->greedy);
    return ch_repeat;
  }else{
    return ch;
  }
}

ast_node_capture* ast_parse_capture(c_ustr& str, size_t& pos, ast_parse_global_state& gst);

void ast_parse_seq(c_ustr& str, size_t& pos, ast_parse_global_state& gst, vector<ast_node*>& _out){
  ast_node* r_node;
  while(pos<str.size){
    switch(str[pos]){
    case '[':
    case '(':
      r_node= ast_parse_capture(str,pos,gst);
      break;
    case '|':
      if(gst.at_square_bracket) {r_node= ast_parse_char(str,pos,gst); break;}
    case ']':
    case ')':
      return;
    default:
      r_node= ast_parse_char_may_repeat(str,pos,gst);
    }
    _out.push_back(r_node);
  }
  return;
}
ast_node_capture* ast_parse_capture(c_ustr& str, size_t& pos, ast_parse_global_state& gst){
  ast_node_capture* ret= new ast_node_capture;
  bool old_at_square_bracket= gst.at_square_bracket;
  if(str[pos]=='('){
    pos++;
    gst.at_square_bracket= false;
    //
    ret->group_index= gst.capture_index++;
    //groups_name
    if(str[pos]=='?'){
      pos++;
      switch(str[pos]){
	case '<':
	  if(true){
	    pos++;
	    size_t name_base=pos;
	    size_t name_size=0;
	    while(str[pos+name_size]!='>'){
	      if(str[pos+name_size]==')'){
		throw "debug unknow";
	      }
	      name_size++;
	    }
	    ret->group_name.set(str.base+name_base, name_size); 
	    pos+= name_size;
	    if(str[pos]!='>'){ throw "debug unknow"; }
	    pos++;
	  }
	  break;
      }
    }
    do{ 
      if(str[pos]=='|') pos++;
      ast_node_seq* cur_seq= new ast_node_seq;
      ast_parse_seq(str,pos,gst,cur_seq->sub);
      ret->sub_branchs.push_back(cur_seq);
    }while(str[pos]=='|');
    
    if(str[pos]!=')'){ 
      throw "debug unknow";
    }
    pos++;
  }
  else if(str[pos]=='['){
    pos++;
    gst.at_square_bracket= true;
    if(str[pos]=='^'){
      pos++;
      ret->caret_mark= true;
    }
    ast_parse_seq(str,pos,gst,ret->sub_branchs);
    if(str[pos]==']'){pos++;}
    else{ 
      throw "debug unknow.";
    }
  }
  ast_parse_repeat(str,pos, ret->repeat_min, ret->repeat_max, ret->greedy);
  gst.at_square_bracket= old_at_square_bracket;
  return ret;
}

ast_node* ast_parse(c_ustr& str, size_t& pos){
  ast_parse_global_state gst;
  gst.capture_index=0;
  gst.at_square_bracket=false;
  
  ast_node_capture* ret= new ast_node_capture;
  ret->group_index= gst.capture_index++;
  do{ 
    if(str[pos]=='|') pos++;
    ast_node_seq* cur_seq= new ast_node_seq;
    ast_parse_seq(str,pos,gst,cur_seq->sub);
    ret->sub_branchs.push_back(cur_seq);
  }while(pos<str.size && str[pos]=='|');
  return ret;
}

void ast_node_capture::print(ostream& s){
    s<<"(";
    if(!group_name.is_null()){
      s<<"?<";
      group_name.print(s);
      s<<">";
    }
    //if(group_index!=-1){s<<"?@"<<group_index<<"@";}
    if(caret_mark) {s<<"^";}
    for(size_t i=0; i<sub_branchs.size(); i++){
      if(i!=0) s<<"|";
      sub_branchs[i]->print(s);
    }
    s<<")";
    if(repeat_min==1 && repeat_max==1){}
    else if(repeat_min==0 && repeat_max==1) {s<<"?";}
    else if(repeat_min==0 && repeat_max==std::numeric_limits<size_t>::max()) {s<<"*";}
    else if(repeat_min==1 && repeat_max==std::numeric_limits<size_t>::max()) {s<<"+";}
    else{
      s<<"{"<<repeat_min<<",";
      if(repeat_max!=std::numeric_limits<size_t>::max()) {s<<repeat_max;}
      s<<"}";
    }
  }