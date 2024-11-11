#ifndef SEMANT_H_
#define SEMANT_H_

#include <assert.h>
#include <iostream>  
#include "cool-tree.h"
#include "stringtab.h"
#include "symtab.h"
#include "list.h"
#include <map>
#include <set>

#define TRUE 1
#define FALSE 0

class ClassTable;
typedef ClassTable *ClassTableP;

// This is a structure that may be used to contain the semantic
// information such as the inheritance graph.  You may use it or not as
// you like: it is only here to provide a container for the supplied
// methods.

class ClassTable {
private:
  int semant_errors;
  void install_basic_classes();
  void install_user_classes(Classes);
  void build_inheritance_graph(Classes);
  bool check_parents_and_inheritance();
  std::map<Symbol, Symbol> parent_type_of;
  ostream& error_stream;

public:
  ClassTable(Classes);
  int errors() { return semant_errors; }
  ostream& semant_error();
  ostream& semant_error(Class_ c);
  ostream& semant_error(Symbol filename, tree_node *t);
  ostream& semant_error(tree_node *t);

  std::map<Symbol, Class_> inh_graph;
  SymbolTable<Symbol, Symbol> *object_table;
  SymbolTable<Symbol, Symbol> *method_table;
  Symbol lub(Symbol, Symbol);

  bool is_subtype_of(Symbol, Symbol);
  bool is_type_defined(Symbol);
  bool is_primitive(Symbol);
  Symbol get_parent_type_of(Symbol);
};


#endif

