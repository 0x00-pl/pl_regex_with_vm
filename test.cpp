#include <iostream>

#include "src/str_convert.h"
#include "src/ast.h"

using namespace std;

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  std::cout << "Hello, world!" << std::endl<< std::endl;
  size_t pos=0;
  c_ustr patt("[12]|3(?<name>4[^56])(7)8\\29");
  ast_node* patt_ast= ast_parse(patt,pos);
  patt_ast->print(cout);
  cout<<endl;
  delete patt_ast;
  return 0;
}
