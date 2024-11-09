

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"


extern int semant_debug;
extern char *curr_filename;

ClassTable *classtable;

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
static Symbol 
    arg,
    arg2,
    Bool,
    concat,
    cool_abort,
    copy,
    Int,
    in_int,
    in_string,
    IO,
    length,
    Main,
    main_meth,
    No_class,
    No_type,
    Object,
    out_int,
    out_string,
    prim_slot,
    self,
    SELF_TYPE,
    Str,
    str_field,
    substr,
    type_name,
    val;

SymbolTable<Symbol ,Symbol> *Environment = new SymbolTable<Symbol, Symbol>;

//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
    arg         = idtable.add_string("arg");
    arg2        = idtable.add_string("arg2");
    Bool        = idtable.add_string("Bool");
    concat      = idtable.add_string("concat");
    cool_abort  = idtable.add_string("abort");
    copy        = idtable.add_string("copy");
    Int         = idtable.add_string("Int");
    in_int      = idtable.add_string("in_int");
    in_string   = idtable.add_string("in_string");
    IO          = idtable.add_string("IO");
    length      = idtable.add_string("length");
    Main        = idtable.add_string("Main");
    main_meth   = idtable.add_string("main");
    //   _no_class is a symbol that can't be the name of any 
    //   user-defined class.
    No_class    = idtable.add_string("_no_class");
    No_type     = idtable.add_string("_no_type");
    Object      = idtable.add_string("Object");
    out_int     = idtable.add_string("out_int");
    out_string  = idtable.add_string("out_string");
    prim_slot   = idtable.add_string("_prim_slot");
    self        = idtable.add_string("self");
    SELF_TYPE   = idtable.add_string("SELF_TYPE");
    Str         = idtable.add_string("String");
    str_field   = idtable.add_string("_str_field");
    substr      = idtable.add_string("substr");
    type_name   = idtable.add_string("type_name");
    val         = idtable.add_string("_val");
}



ClassTable::ClassTable(Classes classes) : semant_errors(0) , error_stream(cerr) {

    /* Fill this in */

    install_basic_classes();
    install_user_classes(classes);
    build_inheritance_graph(classes);
    check_inheritance(classes);
}

void ClassTable::install_basic_classes() {

    // The tree package uses these globals to annotate the classes built below.
    yylineno = 0;
    Symbol filename = stringtable.add_string("<basic class>");

    // The following demonstrates how to create dummy parse trees to
    // refer to basic Cool classes.  There's no need for method
    // bodies -- these are already built into the runtime system.
    
    // IMPORTANT: The results of the following expressions are
    // stored in local variables.  You will want to do something
    // with those variables at the end of this method to make this
    // code meaningful.

    // 
    // The Object class has no parent class. Its methods are
    //        abort() : Object    aborts the program
    //        type_name() : Str   returns a string representation of class name
    //        copy() : SELF_TYPE  returns a copy of the object
    //
    // There is no need for method bodies in the basic classes---these
    // are already built in to the runtime system.

    Class_ Object_class =
	class_(Object, 
	       No_class,
	       append_Features(
			       append_Features(
					       single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
					       single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
			       single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
	       filename);

    // 
    // The IO class inherits from Object. Its methods are
    //        out_string(Str) : SELF_TYPE       writes a string to the output
    //        out_int(Int) : SELF_TYPE            "    an int    "  "     "
    //        in_string() : Str                 reads a string from the input
    //        in_int() : Int                      "   an int     "  "     "
    //
    Class_ IO_class = 
	class_(IO, 
	       Object,
	       append_Features(
			       append_Features(
					       append_Features(
							       single_Features(method(out_string, single_Formals(formal(arg, Str)),
										      SELF_TYPE, no_expr())),
							       single_Features(method(out_int, single_Formals(formal(arg, Int)),
										      SELF_TYPE, no_expr()))),
					       single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
			       single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
	       filename);  

    //
    // The Int class has no methods and only a single attribute, the
    // "val" for the integer. 
    //
    Class_ Int_class =
	class_(Int, 
	       Object,
	       single_Features(attr(val, prim_slot, no_expr())),
	       filename);

    //
    // Bool also has only the "val" slot.
    //
    Class_ Bool_class =
	class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename);

    //
    // The class Str has a number of slots and operations:
    //       val                                  the length of the string
    //       str_field                            the string itself
    //       length() : Int                       returns length of the string
    //       concat(arg: Str) : Str               performs string concatenation
    //       substr(arg: Int, arg2: Int): Str     substring selection
    //       
    Class_ Str_class =
	class_(Str, 
	       Object,
	       append_Features(
			       append_Features(
					       append_Features(
							       append_Features(
									       single_Features(attr(val, Int, no_expr())),
									       single_Features(attr(str_field, prim_slot, no_expr()))),
							       single_Features(method(length, nil_Formals(), Int, no_expr()))),
					       single_Features(method(concat, 
								      single_Formals(formal(arg, Str)),
								      Str, 
								      no_expr()))),
			       single_Features(method(substr, 
						      append_Formals(single_Formals(formal(arg, Int)), 
								     single_Formals(formal(arg2, Int))),
						      Str, 
						      no_expr()))),
	       filename);
}

void ClassTable::install_user_classes(Classes classes)
{
    for(int i = classes->first(); classes->more(i); i = classes->next(i)) {
        Class_ c = classes->nth(i);
        Symbol name = c->get_name();
        Symbol parent = c->get_parent_name();
        if(name == SELF_TYPE || name == Object || name == IO || name == Int || name == Bool || name == Str) {
            semant_error(c) << "Classe" << name << " ja foi definida\n";
        } else if(Environment->probe(name) != NULL) {
            semant_error(c) << "Classe já declarada: " << name << "\n";
        } else {
            Environment->addid(name, new Symbol(parent));
        }
    }
}

//Check this thing also
void ClassTable::build_inheritance_graph(Classes classes) {
    for(int i = classes->first(); classes->more(i); i = classes->next(i)) {
        Class_ c = classes->nth(i);
        Symbol name = c->get_name();
        Symbol parent = c->get_parent_name();
        if(name == SELF_TYPE || name == Object || name == IO || name == Int || name == Bool || name == Str) {
            continue;
        } else if(parent == SELF_TYPE || parent == Int || parent == Bool || parent == Str) {
            semant_error(c) << "Classe" << name << " não pode herdar de" << parent << "\n";
        } else if(parent == Object || parent == IO) {
            if(name != parent) {
                semant_error(c) << "Classe" << name << " não pode herdar de" << parent << "\n";
            }
        } else if(Environment->lookup(parent) == NULL) {
            semant_error(c) << "Classe pai não declarada: " << parent << "\n";
        }
    }
}

void ClassTable::check_inheritance(Classes classes)
{
    for(int i = classes->first(); classes->more(i); i = classes->next(i)) {
        Class_ c = classes->nth(i);
        Symbol name = c->get_name();
        Symbol parent = c->get_parent_name();
        if(name == SELF_TYPE || name == Object || name == IO || name == Int || name == Bool || name == Str) {
            semant_error(c) << "Classe" << name << " não pode ser herdada\n";
        } else if(parent == SELF_TYPE || parent == Int || parent == Bool || parent == Str) {
            semant_error(c) << "Classe" << name << " não pode herdar de" << parent << "\n";
        } else if(parent == Object || parent == IO) {
            if(name != parent) {
                semant_error(c) << "Classe" << name << " não pode herdar de" << parent << "\n";
            }
        } else if(Environment->lookup(parent) == NULL) {
            semant_error(c) << "Classe pai não declarada: " << parent << "\n";
        }
    }
}

// Implementation of Least Upper Bound
Symbol ClassTable::lub(Symbol l1, Symbol l2){
    if(l1 == SELF_TYPE && l2 == SELF_TYPE) {
        return SELF_TYPE;
    } else if(l1 == SELF_TYPE) {
        return l2;
    } else if(l2 == SELF_TYPE) {
        return l1;
    } else if(l1 == l2) {
        return l1;
    } else {
        Symbol p1 = l1;
        Symbol p2 = l2;
        while(p1 != No_class) {
            while(p2 != No_class) {
                if(p1 == p2) {
                    return p1;
                }
                p2 = *Environment->lookup(p2);
            }
            p1 = *Environment->lookup(p1);
            p2 = l2;
        }
        return Object;
    }
}

////////////////////////////////////////////////////////////////////
//
// semant_error is an overloaded function for reporting errors
// during semantic analysis.  There are three versions:
//
//    ostream& ClassTable::semant_error()                
//
//    ostream& ClassTable::semant_error(Class_ c)
//       print line number and filename for `c'
//
//    ostream& ClassTable::semant_error(Symbol filename, tree_node *t)  
//       print a line number and filename
//
///////////////////////////////////////////////////////////////////

ostream& ClassTable::semant_error(Class_ c)
{                                                             
    return semant_error(c->get_filename(),c);
}

ostream& ClassTable::semant_error(tree_node *t) {
    error_stream << ":" << t->get_line_number() << ": ";
    return semant_error();
}

ostream& ClassTable::semant_error(Symbol filename, tree_node *t)
{
    error_stream << filename << ":" << t->get_line_number() << ": ";
    return semant_error();
}

ostream& ClassTable::semant_error()                  
{                                                 
    semant_errors++;                            
    return error_stream;
} 

Symbol object_class::check_type() {
    if(name == self) {
        type = SELF_TYPE;
    } else if(Environment->lookup(name) != NULL) {
        type = *Environment->lookup(name);
    } else {
        classtable->semant_error(name, this) << "Tipo não declarado: " << name << "\n" ;
        type = No_type;
    };

    return type;
}

Symbol no_expr_class::check_type() {
    type = No_type;
    return type;
}

Symbol isvoid_class::check_type() {
    e1->check_type();
    type = Bool;
    return type;
}

Symbol new__class::check_type() {
    if(type_name == SELF_TYPE) {
        type = SELF_TYPE;
    } else if(Environment->lookup(type_name) != NULL) {
        type = type_name;
    } else {
        classtable->semant_error(type_name, this) << "Tipo não declarado: " << type_name << "\n";
        type = No_type;
    }
    return type;
}

Symbol string_const_class::check_type() {
    type = Str;
    return type;
}

Symbol bool_const_class::check_type() {
    type = Bool;
    return type;
}

Symbol int_const_class::check_type() {
    type = Int;
    return type;
}

Symbol comp_class::check_type() {
    
    if(e1->check_type() != Bool) {
        classtable->semant_error(this) << "Operação de negação aplicada a um tipo não-booleano\n";
    }
    type = Bool;
    return type;
}

Symbol leq_class::check_type() {
    if(e1->check_type() != Int || e2->check_type() != Int) {
        classtable->semant_error(this) << "Operação de comparação aplicada a um tipo não-inteiro\n";
    }
    type = Bool;
    return type;
}

Symbol eq_class::check_type() {
    bool isPrimitive1 = e1->check_type() == Int || e1->check_type() == Bool || e1->check_type() == Str;
    bool isPrimitive2 = e2->check_type() == Int || e2->check_type() == Bool || e2->check_type() == Str;
    if(e1->check_type() != e2->check_type() && (isPrimitive1 && isPrimitive2)) {
        classtable->semant_error(this) << "Operação de comparação aplicada a tipos diferentes\n";
    }
    type = Bool;
    return type;
}

Symbol lt_class::check_type() {
    if(e1->check_type()== Int && e2->check_type() == Int) {
        type = Bool;
        return type;
    } else{
        type = Object;
        classtable->semant_error(this) << "Operação de comparação aplicada a um tipo não-inteiro\n";
    }

   return type;
}

Symbol neg_class::check_type() {
    if(e1->check_type() != Int) {
        classtable->semant_error(this) << "Operação de negação aplicada a um tipo não-inteiro\n";
    }
    type = Int;
    return type;
}

Symbol divide_class::check_type() {
    if(e1->check_type() != Int || e2->check_type() != Int) {
        classtable->semant_error(this) << "Operação de divisão aplicada a um tipo não-inteiro\n";
    }
    type = Int;
    return type;
}

Symbol mul_class::check_type() {
    if(e1->check_type() != Int || e2->check_type() != Int) {
        classtable->semant_error(this) << "Operação de multiplicação aplicada a um tipo não-inteiro\n";
    }
    type = Int;
    return type;
}

Symbol sub_class::check_type() {
    if(e1->check_type() != Int || e2->check_type() != Int) {
        classtable->semant_error(this) << "Operação de subtração aplicada a um tipo não-inteiro\n";
    }
    type = Int;
    return type;
}

Symbol plus_class::check_type() {
    if(e1->check_type() != Int || e2->check_type() != Int) {
        classtable->semant_error(this) << "Operação de adição aplicada a um tipo não-inteiro\n";
    }
    type = Int;
    return type;
}

Symbol let_class::check_type() {
    Environment->enterscope();
    Environment->addid(identifier, &type_decl);

    if(identifier == self) {
        classtable->semant_error(this) << "Identificador self não pode ser usado em uma declaração let\n";
    }
    if(Environment->lookup(identifier) != NULL) {
        classtable->semant_error(this) << "Identificador já declarado: " << identifier << "\n";
    }
    if(type_decl == SELF_TYPE) {
        if(init->check_type() != SELF_TYPE) {
            classtable->semant_error(this) << "Tipo de inicialização não corresponde ao tipo declarado\n";
        }
    } else if(type_decl != No_type) {
        if(init->check_type() != type_decl) {
            classtable->semant_error(this) << "Tipo de inicialização não corresponde ao tipo declarado\n";
        }
    }
    
    type = body->check_type();
    Environment->exitscope();
    return type;
}

Symbol block_class::check_type() {
    for(int i = body->first(); body->more(i); i = body->next(i)) {
        type = body->nth(i)->check_type();
    }
    return type;
}

Symbol typcase_class::check_type() {
    type = No_type;
    Symbol type1 = cases->nth(0)->check_type();
    for(int i = cases->first(); cases->more(i); i = cases->next(i)) {
        Symbol type2 = cases->nth(i)->check_type();
        type = classtable->lub(type1, type2);
    }
    return type;
}

Symbol loop_class::check_type() {
    if(pred->check_type() != Bool) {
        classtable->semant_error(this) << "Condição do loop não é booleana\n";
    }
    body->check_type();
    type = Object;
    return type;
}

Symbol cond_class::check_type() {
    if(pred->check_type() != Bool) {
        classtable->semant_error(this) << "Condição do if não é booleana\n";
    }
    Symbol type1 = then_exp->check_type();
    Symbol type2 = else_exp->check_type();
    type = classtable->lub(type1, type2);
    return type;
}

//Review these three
Symbol dispatch_class::check_type() {
    Symbol type1 = expr->check_type();
    if(type1 == SELF_TYPE) {
        type1 = *Environment->lookup(self);
    }
    if(Environment->lookup(type1) == NULL) {
        classtable->semant_error(this) << "Tipo não declarado: " << type1 << "\n";
    }
    if(Environment->lookup(type1)->lookup(name) == NULL) {
        classtable->semant_error(this) << "Método não declarado: " << name << "\n";
    }
    Symbol type2 = Environment->lookup(type1)->lookup(name);
    if(type2 == SELF_TYPE) {
        type = SELF_TYPE;
    } else {
        type = type2;
    }
    return type;
}

Symbol static_dispatch_class::check_type() {
    Symbol type1 = expr->check_type();
    if(type1 == SELF_TYPE) {
        type1 = *Environment->lookup(self);
    }
    if(Environment->lookup(type1) == NULL) {
        classtable->semant_error(this) << "Tipo não declarado: " << type1 << "\n";
    }
    if(Environment->lookup(type1)->lookup(name) == NULL) {
        classtable->semant_error(this) << "Método não declarado: " << name << "\n";
    }
    Symbol type2 = Environment->lookup(type1)->lookup(name);
    if(type2 == SELF_TYPE) {
        type = SELF_TYPE;
    } else {
        type = type2;
    }
    if(type != return_type) {
        classtable->semant_error(this) << "Tipo de retorno não corresponde ao tipo declarado\n";
    }
    return type;
}

Symbol assign_class::check_type() {
    if(name == self) {
        classtable->semant_error(this) << "Atribuição ao self não é permitida\n";
    }
    if(Environment->lookup(name) == NULL) {
        classtable->semant_error(this) << "Identificador não declarado: " << name << "\n";
    }
    Symbol type1 = Environment->lookup(name);
    Symbol type2 = expr->check_type();
    if(type1 == SELF_TYPE) {
        type1 = *Environment->lookup(self);
    }
    if(type2 == SELF_TYPE) {
        type2 = *Environment->lookup(self);
    }
    if(type1 != type2) {
        classtable->semant_error(this) << "Tipo de atribuição não corresponde ao tipo declarado\n";
    }
    type = type2;
    return type;
}

Symbol branch_class::check_type() {
    Environment->enterscope();
    Environment->addid(name, &type_decl);
    type = expr->check_type();
    Environment->exitscope();
    return type;
}




/*   This is the entry point to the semantic checker.

     Your checker should do the following two things:

     1) Check that the program is semantically correct
     2) Decorate the abstract syntax tree with type information
        by setting the `type' field in each Expression node.
        (see `tree.h')

     You are free to first do 1), make sure you catch all semantic
     errors. Part 2) can be done in a second stage, when you want
     to build mycoolc.
 */
void program_class::semant()
{
    initialize_constants();

    /* ClassTable constructor may do some semantic analysis */
    classtable = new ClassTable(classes);

    /* some semantic analysis code may go here */

    if (classtable->errors()) {
	cerr << "Compilation halted due to static semantic errors." << endl;
	exit(1);
    }
}
