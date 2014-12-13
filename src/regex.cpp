#include "regex.h"
#include "ast.h"
#include "il.h"
#include "vm.h"


pl_regex::pl_regex(string s){
  size_t pos=0;
  c_ustr patt(s);
  ast_parse_global_state gst;
  ast_node* patt_ast= ast_parse(patt,pos,gst);
  
#ifdef PL_DEBUG
  cout<<"patt is: "<<s<<endl;
  cout<<"ast  is: ";
  patt_ast->print(cout);
  cout<<endl;
#endif
  
  cmd_item_builder beg(cmd_type::enum_nop);
  cmd_item_builder end(cmd_type::enum_success);
  end.next= nullptr;
  
  bulid_il_ast_node(patt_ast, &beg, &end);
  delete patt_ast;
  
  size_t cmd_count=0;
  for(cmd_item_builder* p= &beg; p!=nullptr; p=p->next){
    p->address= cmd_count++;
  }
  
  for(cmd_item_builder* p= &beg; p!=nullptr; p=p->next){
    if(p->argp!=nullptr) p->ci.arg1= p->argp->address;
  }
  
  _compired.capture_pool_size= gst.capture_index;
  _compired.loop_pool_size= gst.repeat_index;
  _compired.loop_min= gst.loop_min;
  _compired.loop_max= gst.loop_max;
  _compired.loop_greedy= gst.loop_greedy;
  _compired.capture_id_name= gst.capture_id_name;
  
  _compired.cmds.resize(cmd_count);
  size_t iter=0;
  for(cmd_item_builder* p= &beg; p!=nullptr; p=p->next){
    _compired.cmds[iter]= p->ci;
    iter++;
  }
  
  for(cmd_item_builder *pnext,*p= beg.next; p!=&end; p=pnext){
    pnext=p->next;
    delete p;
  }
  
#ifdef PL_DEBUG
  cout<<"il cmds:"<<endl;
  for(size_t i=0; i<_compired.cmds.size(); i++){
    cout<<"["<<i<<"]: \t"<< cmd_type_names(_compired.cmds[i].cmd)<< _compired.cmds[i].arg1<<endl;
  }
  cout<<endl;
#endif
}

bool pl_regex::match(string s){
  c_ustr text(s);
  vm_state _vm(_compired, text, captures);
  bool ret=_vm.run();
  
#ifdef PL_DEBUG
  cout<<endl;
  for(size_t i=0; i<captures.size(); i++){
    cout<<"capture \\"<<captures[i].capture_id
    <<(captures[i]._in_or_out?" begin ":"  end  ")
    <<"at "<<captures[i].p_text<<endl;
  }
  cout<<endl;
#endif
  
  return ret;
}
