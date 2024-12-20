//
// The following include files must come first.

#ifndef COOL_TREE_HANDCODE_H
#define COOL_TREE_HANDCODE_H

#include "cool-io.h"
#include "tree.h"
#include "cool.h"
#include "stringtab.h"

extern int yylineno;

inline Boolean copy_Boolean(Boolean b) {return b; }
inline void assert_Boolean(Boolean) {}
inline void dump_Boolean(ostream& stream, int padding, Boolean b)
	{ stream << pad(padding) << (int) b << "\n"; }

void dump_Symbol(ostream& stream, int padding, Symbol b);
void assert_Symbol(Symbol b);
Symbol copy_Symbol(Symbol b);

class Program_class;
typedef Program_class *Program;
class Class__class;
typedef Class__class *Class_;
class Feature_class;
typedef Feature_class *Feature;
class Formal_class;
typedef Formal_class *Formal;
class Expression_class;
typedef Expression_class *Expression;
class Case_class;
typedef Case_class *Case;

typedef list_node<Class_> Classes_class;
typedef Classes_class *Classes;
typedef list_node<Feature> Features_class;
typedef Features_class *Features;
typedef list_node<Formal> Formals_class;
typedef Formals_class *Formals;
typedef list_node<Expression> Expressions_class;
typedef Expressions_class *Expressions;
typedef list_node<Case> Cases_class;
typedef Cases_class *Cases;

#define Program_EXTRAS                          \
virtual void semant() = 0;			\
virtual void dump_with_types(ostream&, int) = 0; 



#define program_EXTRAS                          \
void semant();     				\
void dump_with_types(ostream&, int);            

#define Class__EXTRAS                   \
virtual Symbol get_filename() = 0;      \
virtual Symbol get_name() = 0;          \
virtual Symbol get_parent_name() = 0;          \
virtual Features get_features() = 0;          \
virtual void dump_with_types(ostream&,int) = 0; 


#define class__EXTRAS                        \
Symbol get_filename() { return filename; }   \
Symbol get_name() { return name; }           \
Symbol get_parent_name() { return parent; }  \
Features get_features() { return features; } \
void dump_with_types(ostream&,int);          


#define Feature_EXTRAS                          \
virtual bool is_method() = 0;                   \
virtual bool is_attr() = 0;                     \
virtual void dump_with_types(ostream&,int) = 0;


#define Feature_SHARED_EXTRAS                                       \
void dump_with_types(ostream&,int);    

#define method_EXTRAS                            \
bool is_method() { return true; }                \
bool is_attr() { return false; }                 \
Symbol get_name() { return name; }               \
Formals get_formals() { return formals; }        \
Symbol get_return_type() { return return_type; } \
Expression get_expression() { return expr; }     \
Symbol check_type();

#define attr_EXTRAS                               \
bool is_method() { return false; }                \
bool is_attr() { return true; }                   \
Symbol get_name() { return name; }                \
Symbol get_type() { return type_decl; }           \
Expression get_init_expression() { return init; } \
Symbol check_type();

#define Formal_EXTRAS                              \
virtual Symbol get_type() = 0;           				   \
virtual Symbol get_name() = 0;             				   \
virtual void dump_with_types(ostream&,int) = 0;


#define formal_EXTRAS                           \
Symbol get_name() { return name; }                \
Symbol get_type() { return type_decl; }           \
void dump_with_types(ostream&,int);


#define Case_EXTRAS                              \
Symbol type;                                     \
Symbol get_type() { return type; }               \
virtual Symbol check_type() = 0;                  \
virtual void dump_with_types(ostream& ,int) = 0;


#define branch_EXTRAS                        \
Symbol get_type_decl() { return type_decl; } \
Symbol check_type();                 \
void dump_with_types(ostream& ,int);


#define Expression_EXTRAS                    \
Symbol type;                                 \
Symbol get_type() { return type; }           \
Expression set_type(Symbol s) { type = s; return this; } \
virtual Symbol check_type() = 0;                  \
virtual void dump_with_types(ostream&,int) = 0;  \
void dump_type(ostream&, int);               \
Expression_class() { type = (Symbol) NULL; }

#define Expression_SHARED_EXTRAS           \
Symbol check_type();                 \
void dump_with_types(ostream&,int); 

#endif
