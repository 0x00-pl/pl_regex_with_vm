#include <iostream>

#include "src/str_convert.h"
#include "src/ast.h"
#include "src/il.h"
#include "src/regex.h"

using namespace std;

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  std::cout << "Hello, world!" << std::endl<< std::endl;
  /*
  size_t pos=0;
  c_ustr patt("123456");
  ast_parse_global_state gst;
  ast_node* patt_ast= ast_parse(patt,pos,gst);
  patt_ast->print(cout);
  cout<<endl;
  
  cmd_item_builder beg;
  cmd_item_builder end;
  beg.ci.cmd=end.ci.cmd=999;
  
  bulid_il_ast_node(patt_ast, &beg, &end);
  
  for(cmd_item_builder* p= &beg; p!=&end; p=p->next){
    cout<<p->ci.cmd<<"  "<<cmd_type_names(p->ci.cmd)<<p->ci.arg1<<endl;
  }
  
  delete patt_ast;
  */
  
  pl_regex plre("/hello/(.*?)");
  bool res= plre.match("/hello/123456");
  
  cout<<"match result: "<<res<<endl;
  return 0;
}

/*
 
/a*abc?xyz+pqr{3}ab{2,}xy{4,5}pq{0,6}AB{0,}zz/
    abxyzpqrrrabbxyyyypqAzz
    abxyzpqrrrabbxyyyypqAzz
    aabxyzpqrrrabbxyyyypqAzz
    aaabxyzpqrrrabbxyyyypqAzz
    aaaabxyzpqrrrabbxyyyypqAzz
    abcxyzpqrrrabbxyyyypqAzz
    aabcxyzpqrrrabbxyyyypqAzz
    aaabcxyzpqrrrabbxyyyypAzz
    aaabcxyzpqrrrabbxyyyypqAzz
    aaabcxyzpqrrrabbxyyyypqqAzz
    aaabcxyzpqrrrabbxyyyypqqqAzz
    aaabcxyzpqrrrabbxyyyypqqqqAzz
    aaabcxyzpqrrrabbxyyyypqqqqqAzz
    aaabcxyzpqrrrabbxyyyypqqqqqqAzz
    aaaabcxyzpqrrrabbxyyyypqAzz
    abxyzzpqrrrabbxyyyypqAzz
    aabxyzzzpqrrrabbxyyyypqAzz
    aaabxyzzzzpqrrrabbxyyyypqAzz
    aaaabxyzzzzpqrrrabbxyyyypqAzz
    abcxyzzpqrrrabbxyyyypqAzz
    aabcxyzzzpqrrrabbxyyyypqAzz
    aaabcxyzzzzpqrrrabbxyyyypqAzz
    aaaabcxyzzzzpqrrrabbxyyyypqAzz
    aaaabcxyzzzzpqrrrabbbxyyyypqAzz
    aaaabcxyzzzzpqrrrabbbxyyyyypqAzz
    aaabcxyzpqrrrabbxyyyypABzz
    aaabcxyzpqrrrabbxyyyypABBzz
    >>>aaabxyzpqrrrabbxyyyypqAzz
    >aaaabxyzpqrrrabbxyyyypqAzz
    >>>>abcxyzpqrrrabbxyyyypqAzz
    *** Failers
    abxyzpqrrabbxyyyypqAzz
    abxyzpqrrrrabbxyyyypqAzz
    abxyzpqrrrabxyyyypqAzz
    aaaabcxyzzzzpqrrrabbbxyyyyyypqAzz
    aaaabcxyzzzzpqrrrabbbxyyypqAzz
    aaabcxyzpqrrrabbxyyyypqqqqqqqAzz
 
 */
