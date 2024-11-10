

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"


extern int semant_debug;
extern char *curr_filename;

ClassTable *classtable;

Symbol current_class_name;
Class_ current_class_definition;

std::map<Symbol, method_class*> current_class_methods;
std::map<Symbol, attr_class*> current_class_attrs;

std::map<Symbol, std::map<Symbol, method_class*>> class_methods;
std::map<Symbol, std::map<Symbol, attr_class*>> class_attrs;

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
    cout << "Installed basic" << endl;
    build_inheritance_graph(classes);
    cout << "Built Inheritance" << endl;
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

    inh_graph.insert(std::pair<Symbol, Class_>(Object, Object_class));
	inh_graph.insert(std::pair<Symbol, Class_>(IO, IO_class));
	inh_graph.insert(std::pair<Symbol, Class_>(Int, Int_class));
	inh_graph.insert(std::pair<Symbol, Class_>(Bool, Bool_class));
	inh_graph.insert(std::pair<Symbol, Class_>(Str, Str_class));
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

//=================================================================================
// Inheriteance
//=================================================================================

void ClassTable::build_inheritance_graph(Classes classes) {
    bool main_class_not_defined_error = false;
    bool class_redefinition_error = false;
    for(int i=classes->first(); classes->more(i); i=classes->next(i)) {
        Class_ cur_class=classes->nth(i);
        
        // Check if is Main class
        if(cur_class->get_name()==Main)
            main_class_not_defined_error = false;
        
        // Check class redefinition
        if(cur_class->get_name()==SELF_TYPE || inh_graph.find(cur_class->get_name())!=inh_graph.end()) {
            if(cur_class->get_name()==IO || cur_class->get_name()==Int || cur_class->get_name()==Str || cur_class->get_name()==Bool || cur_class->get_name()==Object || cur_class->get_name()==SELF_TYPE)
                semant_error(cur_class)<<"Basic Class Redefinition "<<cur_class->get_name()<<"."<<endl;
            else
                semant_error(cur_class)<<"Class "<<(cur_class->get_name())<<" was previously defined."<<endl;
            class_redefinition_error=true;
        } else
            inh_graph.insert(std::pair<Symbol, Class_>(cur_class->get_name(), cur_class));
    }
    
    if(!class_redefinition_error)
        if(check_parents_and_inheritance())
            if(main_class_not_defined_error)
                semant_error()<<"Class Main is not defined."<<endl;
}

bool ClassTable::check_parents_and_inheritance() {
    
    bool error_parents=false;
    bool error_inhe=false;
    for(std::map<Symbol, Class_>::iterator it=inh_graph.begin(); it!=inh_graph.end(); it++) {

        // Check valid partents
        Symbol child_class_symbol=it->first, parent_class_symbol=it->second->get_parent_name();
        if(child_class_symbol==Object)
            continue;
        
        //check if parent is defined
        if(inh_graph.find(parent_class_symbol)==inh_graph.end()) {
            semant_error(it->second)<<"Class "<<child_class_symbol<<" inherits from an undefined class "<<parent_class_symbol<<"."<<endl;
            error_parents=true;
        }
        
        //check if inheritance from Int, String or Bool
        if(parent_class_symbol==Int || parent_class_symbol==Str || parent_class_symbol==Bool) {
            semant_error(it->second)<<"Class "<<child_class_symbol<<" cannot inherit class "<<parent_class_symbol<<"."<<endl;
            error_parents=true;
        }

        // Check inheritence cycles.
        Symbol cur_class, slow_class_ref, fast_class_ref;
        cur_class=slow_class_ref=fast_class_ref=it->first;
        
        while(slow_class_ref!=Object && fast_class_ref!=Object && inh_graph[fast_class_ref]->get_parent_name()!=Object) {
            slow_class_ref=inh_graph[slow_class_ref]->get_parent_name();
            fast_class_ref=inh_graph[inh_graph[fast_class_ref]->get_parent_name()]->get_parent_name();
            
            //if loop is found
            if(slow_class_ref==fast_class_ref) {
                semant_error(inh_graph[cur_class])<<"Class "<<cur_class<<", or an ancestor of "<<cur_class<<", is involved in an inheritance cycle."<<endl;
                error_inhe=true;
                break;
            }
        }
    }

    if (error_inhe || error_parents) {
        return false;
    } else {
        return true;
    }
}

bool ClassTable::is_type_defined(Symbol name) {

    std::map<Symbol, Class_>::iterator it;

    it  = inh_graph.find(name);

    if (it != inh_graph.end()){
        return true;
    }

    return false;
}

//=================================================================================
// Semant Error
//=================================================================================

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

////////////////////////////////////////////////////////////////////
//                          CLASS UTILITIES
////////////////////////////////////////////////////////////////////

std::map<Symbol, method_class*> get_class_methods(Class_ class_definition) {
    std::map<Symbol, method_class*> class_methods;
    Symbol class_name = class_definition->get_name();
    Features class_features = class_definition->get_features();

    for (int i = class_features->first(); class_features->more(i); i = class_features->next(i)) 
    {
        Feature feature = class_features->nth(i);

        if (!feature->is_method())
            continue;

        method_class* method = static_cast<method_class*>(feature);
        Symbol method_name = method->get_name();
        
        if (class_methods.find(method_name) != class_methods.end())
        {
            ostream& error_stream = classtable->semant_error(class_definition);
            error_stream << "The method :";
            method_name->print(error_stream);
            error_stream << " has already been defined!\n";
        }
        else
        {
            class_methods[method_name] = method;
        }
    }
    return class_methods;
}

method_class* get_class_method(Symbol class_name, Symbol method_name) {
    std::map<Symbol, method_class*> methods = class_methods[class_name];

    if (methods.find(method_name) == methods.end())
        return nullptr;

    return methods[method_name];
}

attr_class* get_class_attr(Symbol class_name, Symbol attr_name) {
    
    cout << "Entered get class" << endl;
    std::map<Symbol, attr_class*> attrs = class_attrs[class_name];
    
    if (attrs.find(attr_name) == attrs.end()){
        cout << "Nullptr" << endl;
        return nullptr;
    }
        

    cout << "Exiting" << endl;

    return attrs[attr_name];
}

void ensure_class_attributes_are_unique(Class_ class_definition) {
    std::set<Symbol> class_attrs;
    Symbol class_name = class_definition->get_name();
    Features class_features = class_definition->get_features();

    for (int i = class_features->first(); class_features->more(i); i = class_features->next(i)) 
    {
        Feature feature = class_features->nth(i);

        if (!feature->is_attr())
            continue;

        attr_class* attr = static_cast<attr_class*>(feature);
        Symbol attr_name = attr->get_name();
        
        if (class_attrs.find(attr_name) != class_attrs.end())
        {
            classtable->semant_error(class_definition)
                << "The attribute :"
                << attr_name
                << " has already been defined!\n";
        }
        class_attrs.insert(attr_name);
    }
}

std::map<Symbol, attr_class*> get_class_attributes(Class_ class_definition) {
    std::map<Symbol, attr_class*> class_attrs;
    Symbol class_name = class_definition->get_name();
    Features class_features = class_definition->get_features();

    for (int i = class_features->first(); class_features->more(i); i = class_features->next(i)) 
    {
        Feature feature = class_features->nth(i);

        if (!feature->is_attr())
            continue;

        attr_class* attr = static_cast<attr_class*>(feature);
        Symbol attr_name = attr->get_name();
        class_attrs[attr_name] = attr;
    }

    return class_attrs;
}

bool ClassTable::is_subtype_of(Symbol candidate, Symbol desired_type) {
    if (candidate == No_type)
        return true;
        
    if (candidate == SELF_TYPE) {
        if (desired_type == SELF_TYPE)
            return true;
        else
            candidate = current_class_name; 
    }

    Symbol current = candidate;

    while (current != Object && current != desired_type)
        current = Environment->lookup(current) ? *Environment->lookup(current) : Object;

    return current == desired_type;
}

bool ClassTable::is_primitive(Symbol symbol) {
    return (
        symbol == Object ||
        symbol == IO     ||
        symbol == Int    ||
        symbol == Bool   ||
        symbol == Str
    );
}
//=================================================================================
// Check Types
//=================================================================================

void build_attribute_scopes(Class_ current_class) {
    Environment->enterscope();
    std::map<Symbol, attr_class*> attrs = get_class_attributes(current_class);
    for(const auto &x : attrs) {
        attr_class* attr_definition = x.second;
        Environment->addid(
            attr_definition->get_name(), 
            new Symbol(attr_definition->get_type())
        );
    }

    if(current_class->get_name() == Object)
        return;

    Symbol parent_type_name = current_class->get_parent_name();
    Class_ parent_definition = classtable->inh_graph[parent_type_name];
    build_attribute_scopes(parent_definition);
}

void process_attr(Class_ current_class, attr_class* attr) {
    cout << "Entered process attr" << endl;
    if (get_class_attr(current_class->get_name(), attr->get_name()) != nullptr)
    {
        cout << "1" << endl;
        classtable->semant_error(current_class_definition) << " Attribute " << attr->get_name() << " is an attribute of an inherited class.\n";
        cout << "2" << endl;
        cerr << "Compilation halted due to static semantic errors." << endl;
        cout << "3" << endl;
        exit(1);
    }
    Symbol parent_type_name = current_class->get_parent_name();
    if (parent_type_name == No_type)
        return;
    cout << "Still here" << endl;
    Class_ parent_definition = classtable->inh_graph[parent_type_name];
    cout << "here" << endl;
    process_attr(parent_definition, attr);
    cout << "out" << endl;
}

void process_method(Class_ current_class, method_class* original_method, method_class* parent_method) {
    if (parent_method == nullptr)
        return;

    Formals original_method_args = original_method->get_formals();
    Formals parent_method_args = parent_method->get_formals();
    
    int original_formal_ix = 0;
    int parent_formal_ix = 0;
    
    if(original_method->get_return_type() != parent_method->get_return_type()) {
        classtable->semant_error(current_class) 
            << "In redefined method " 
            << original_method->get_name() 
            << ", the return type " 
            << original_method->get_return_type() 
            << " differs from the ancestor method return type "
            << parent_method->get_return_type() 
            << ".\n";
    }

    int original_methods_formals = 0;
    int parent_method_formals = 0;

    while (original_method_args->more(original_methods_formals))
        original_methods_formals = original_method_args->next(original_methods_formals);

    while (parent_method_args->more(parent_method_formals))
        parent_method_formals = parent_method_args->next(parent_method_formals);

    if (original_methods_formals != parent_method_formals) {
        classtable->semant_error(current_class) 
            << "In redefined method " 
            << original_method->get_name() 
            << ", the number of arguments " 
            << "(" << original_methods_formals << ")"
            << " differs from the ancestor method's "
            << "number of arguments "
            << "(" << parent_method_formals << ")"
            << ".\n";
    }

    while (
        original_method_args->more(original_formal_ix) &&
        parent_method_args->more(parent_formal_ix)
    )
    {
        Formal original_formal = original_method_args->nth(original_formal_ix);
        Formal parent_formal = parent_method_args->nth(parent_formal_ix);

        if (original_formal->get_type() != parent_formal->get_type()) {
            classtable->semant_error(current_class) 
                << "In redefined method " 
                << original_method->get_name() 
                << ", the return type of argument " 
                << original_formal->get_type() 
                << " differs from the corresponding ancestor method argument return type "
                << parent_formal->get_type() 
                << ".\n";
        }

        original_formal_ix = original_method_args->next(original_formal_ix);
        parent_formal_ix = parent_method_args->next(parent_formal_ix);
    }

    Symbol parent_type_name =current_class->get_parent_name();

    if (parent_type_name == No_type)
        return;

    Class_ parent_class_definition = classtable->inh_graph[parent_type_name];

    process_method(
        parent_class_definition, 
        original_method,
        get_class_method(
            parent_type_name, 
            original_method->get_name()
        )
    );
}

method_class* lookup_method_in_chain(Symbol class_name, Symbol method_name) {  
    if (class_name == No_type)
        return nullptr;

    method_class* definition = get_class_method(class_name, method_name);
    if (definition)
        return definition;

    Symbol parent_type_name = classtable->inh_graph[class_name]->get_parent_name();
    return lookup_method_in_chain(parent_type_name, method_name);
}

method_class* lookup_method(Symbol class_name, Symbol method_name) {  
    method_class* chain_method = lookup_method_in_chain(class_name, method_name);

    if (chain_method)
        return chain_method;

    if (classtable->is_primitive(class_name)) 
    {
        return get_class_method(class_name, method_name);
    }
    return nullptr;
}

void register_class_and_its_methods(Class_ class_definition) {
    class_methods[class_definition->get_name()] = get_class_methods(class_definition);
    class_attrs[class_definition->get_name()] = get_class_attributes(class_definition);
}

void type_check(Class_ next_class) {
    cout << "Started typecheck" << endl;
    current_class_name = next_class->get_name();
    current_class_definition = next_class;
    current_class_methods = get_class_methods(next_class);
    ensure_class_attributes_are_unique(next_class);
    current_class_attrs = get_class_attributes(next_class);

    cout << "Environment" << endl;
    Environment->enterscope();
    Environment->addid(self, new Symbol(current_class_definition->get_name()));

    cout << "Built attr scopes" << endl;
    build_attribute_scopes(next_class);
    
    cout << "Curr class methods" << endl;
    for (const auto &x : current_class_methods) {
        process_method(next_class, x.second, x.second);
    }

    cout << "Curr class attrs" << endl;
    for (const auto &x : current_class_attrs) {
        Symbol parent_type_name = classtable->inh_graph[current_class_name]->get_parent_name();
        Class_ parent_definition = classtable->inh_graph[parent_type_name];
        cout << "Curr class attrs proc" << endl;
        process_attr(parent_definition, x.second);
    }

    cout << "Curr class attr check" << endl;
    for (const auto &x : current_class_attrs) {
        x.second->check_type();
    }

    cout << "Curr class methods check" << endl;
    for (const auto &x : current_class_methods) {
        x.second->check_type();
    }

    Environment->exitscope();
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

Symbol method_class::check_type() {
    Environment->enterscope();
    std::set<Symbol> defined_arguments;

    for (int formal_ix = formals->first(); formals->more(formal_ix); formal_ix = formals->next(formal_ix))
    {
        Formal argument = formals->nth(formal_ix);
        Symbol argument_name = argument->get_name();
        Symbol argument_type = argument->get_type();

        if(argument_name == self)
            classtable->semant_error(argument) << "'self' cannot be the name of a method argument.\n";
        else if(defined_arguments.find(argument_name) != defined_arguments.end())
            classtable->semant_error(argument) 
                << "The argument " 
                << argument_name 
                << " in the signature of method "
                << get_name()
                << " has already been defined.\n";
        else
        {
           defined_arguments.insert(argument_name);
        }
        
        if (!classtable->is_type_defined(argument_type))
            classtable->semant_error(argument) 
                << "The argument " 
                << argument_name 
                << " in the signature of method "
                << get_name()
                << " has undefined type "
                << argument_type
                << " .\n";
        else
            Environment->addid(argument_name, new Symbol(argument_type));
    }

    Symbol expected_return_type = get_return_type();
    Symbol actual_return_type = this->expr->check_type();

    if (!classtable->is_subtype_of(actual_return_type, expected_return_type))
    {
        classtable->semant_error(this)
            << "Inferred return type "
            << actual_return_type << 
            " of the method " 
            << this->get_name() 
            << " is not compatible with declared return type " 
            << expected_return_type 
            << ".\n";
    }
    
    Environment->exitscope();
    return this->get_return_type();
}

Symbol attr_class::check_type() {
    Expression init_expr = this->get_init_expression();
    Symbol init_expr_type = init_expr->check_type();
    init_expr_type = init_expr_type == SELF_TYPE ? current_class_name : init_expr_type;
    
    if (dynamic_cast<const no_expr_class*>(init_expr) != nullptr)
        return this->get_type();

    if (this->get_name() == self) {
        classtable->semant_error(this) << "'self' cannot be the name of an attribute.\n";
        return this->get_type();
    }
    
    if (!classtable->is_type_defined(this->get_type())) {
        classtable->semant_error(init_expr)
            << "The attribute "
            << this->get_name()
            << " is defined as "
            << this->get_type()
            << " but type "
            << this->get_type()
            << " is undefined. \n";
        return this->get_type();
    }

    bool does_init_type_match_defined_type = classtable->is_subtype_of(
        init_expr_type,
        this->get_type()
    );

    if(!does_init_type_match_defined_type) {
        classtable->semant_error(init_expr)
            << "The attribute "
            << this->get_name()
            << " is defined as "
            << this->get_type()
            << " but is initialized with "
            << init_expr_type
            << ".\n";
    }
    return this->get_type();
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

Symbol dispatch_class::check_type() {

    Symbol expr_type = expr->check_type();

    if (expr_type != SELF_TYPE && !classtable->is_type_defined(expr_type)) {

        classtable->semant_error(this) 
            << "Dispatch on undefined class " 
            << expr_type 
            << ".\n";

        this->set_type(Object);
        return type;
    }
    
    Symbol expr_type_name = expr_type == SELF_TYPE ? current_class_name : expr_type;
    method_class* method_definition = lookup_method(expr_type_name, name);
  
    if (!method_definition) 
    {
        classtable->semant_error(this) 
            << "Dispatch to undefined method " 
            << name 
            << ".\n";
        
        this->set_type(Object);
        return Object;
    }

    Symbol declared_return_type = method_definition->get_return_type();
    Formals declared_method_args = method_definition->get_formals();
    Expressions actual_method_args = this->actual;

    int declared_method_args_count = 0;
    int actual_method_args_count = 0;

    while (declared_method_args->more(declared_method_args_count))
        declared_method_args_count = declared_method_args->next(declared_method_args_count);
    while (actual_method_args->more(actual_method_args_count))
        actual_method_args_count = actual_method_args->next(actual_method_args_count);

    if (declared_method_args_count != actual_method_args_count) {
        classtable->semant_error(this) 
            << "In the dispatch to method " 
            << method_definition->get_name() 
            << ", given number of arguments " 
            << "(" << actual_method_args_count << ")"
            << " differs from the declared method's "
            << "number of arguments "
            << "(" << declared_method_args_count << ")"
            << ".\n";
    }

    int declared_argument_ix = declared_method_args->first();
    int actual_argument_ix = actual_method_args->first();

    Formal declared_argument;
    Expression actual_argument;
    bool is_dispatch_valid = true;

    while (
        actual_method_args->more(actual_argument_ix) && 
        declared_method_args->more(declared_argument_ix)
    )
    {
        actual_argument = actual_method_args->nth(actual_argument_ix);
        declared_argument = declared_method_args->nth(declared_argument_ix);

        Symbol actual_argument_type = actual_argument->check_type();
        Symbol declared_argument_type = declared_argument->get_type();

        if (!classtable->is_subtype_of(actual_argument_type, declared_argument_type)) {
            is_dispatch_valid = false;

            classtable->semant_error(this) 
                << "In the dispatch of the method " 
                << method_definition->get_name() 
                << ", type "
                << actual_argument_type 
                << " of provided argument " 
                << declared_argument->get_name() 
                << " is not compatible with the corresponding signature type " 
                << declared_argument_type 
                << " .\n";
        }

        actual_argument_ix = actual_method_args->next(actual_argument_ix);
        declared_argument_ix = declared_method_args->next(declared_argument_ix);
    }
    
    if (!is_dispatch_valid)
    {
        this->set_type(Object);
        return Object;
    }

    Symbol dispatch_type = declared_return_type == SELF_TYPE ? expr_type : declared_return_type;
    this->set_type(dispatch_type);
    return dispatch_type;
}

Symbol static_dispatch_class::check_type() {
    Symbol expr_type = expr->check_type();

    if (this->type_name != SELF_TYPE && !classtable->is_type_defined(type_name)) {
        classtable->semant_error(this) 
            << "Static dispatch on undefined class " 
            << type_name 
            << ".\n";

        this->set_type(Object);
        return Object;
    }
    if (expr_type != SELF_TYPE && !classtable->is_type_defined(expr_type)) {
        this->set_type(Object);
        return type;
    }

    bool is_dispatch_valid = true;

    if (!classtable->is_subtype_of(expr_type, this->type_name)) {
        is_dispatch_valid = true;
        classtable -> semant_error(this) 
            << "Expression type " 
            << expr_type 
            << " is not compatible with declared static dispatch type " 
            << this->type_name 
            <<  ".\n";
    }
    
    method_class* method_definition = lookup_method(type_name, name);
    if (!method_definition) 
    {
        classtable->semant_error(this) 
            << "Dispatch to undefined method " 
            << name 
            << ".\n";
        
        this->set_type(Object);
        return Object;
    }

    Symbol declared_return_type = method_definition->get_return_type();
    Formals declared_method_args = method_definition->get_formals();
    Expressions actual_method_args = this->actual;

    int declared_method_args_count = 0;
    int actual_method_args_count = 0;

    while (declared_method_args->more(declared_method_args_count))
        declared_method_args_count = declared_method_args->next(declared_method_args_count);
    while (actual_method_args->more(actual_method_args_count))
        actual_method_args_count = actual_method_args->next(actual_method_args_count);

    if (declared_method_args_count != actual_method_args_count) {
        classtable->semant_error(this) 
            << "In the dispatch to method " 
            << method_definition->get_name() 
            << ", given number of arguments " 
            << "(" << actual_method_args_count << ")"
            << " differs from the declared method's "
            << "number of arguments "
            << "(" << declared_method_args_count << ")"
            << ".\n";
    }

    int declared_argument_ix = declared_method_args->first();
    int actual_argument_ix = actual_method_args->first();
    Formal declared_argument;
    Expression actual_argument;

    while (
        actual_method_args->more(actual_argument_ix) && 
        declared_method_args->more(declared_argument_ix)
    )
    {
        actual_argument = actual_method_args->nth(actual_argument_ix);
        declared_argument = declared_method_args->nth(declared_argument_ix);

        Symbol actual_argument_type = actual_argument->check_type();
        Symbol declared_argument_type = declared_argument->get_type();

        if (!classtable->is_subtype_of(actual_argument_type, declared_argument_type)) {
            is_dispatch_valid = false;

            classtable->semant_error(this) 
                << "In the dispatch of the method " 
                << method_definition->get_name() 
                << ", type "
                << actual_argument_type 
                << " of provided argument " 
                << declared_argument->get_name() 
                << " is not compatible with the corresponding signature type " 
                << declared_argument_type 
                << " .\n";
        }

        actual_argument_ix = actual_method_args->next(actual_argument_ix);
        declared_argument_ix = declared_method_args->next(declared_argument_ix);
    }
    
    if (!is_dispatch_valid)
    {
        this->set_type(Object);
        return Object;
    }

    Symbol dispatch_type = declared_return_type == SELF_TYPE ? expr_type : declared_return_type;
    this->set_type(dispatch_type);
    return dispatch_type;
}

Symbol assign_class::check_type() {
    Symbol identifier = name;
    Expression assign_expr = expr;

    if (identifier == self) {
        classtable->semant_error(this) << "Cannot assign to 'self'" << ".\n";
        return Object;
    }

    Symbol* identifier_type = Environment->lookup(identifier);

    if (!identifier_type) {
        classtable->semant_error(this) 
            << "Tried to assign undeclared identifier " 
            << identifier 
            << ".\n";

        this->set_type(Object);
        return this->get_type();
    }

    Symbol assign_expr_type = assign_expr->check_type();

    bool does_assign_conform_declared = classtable->is_subtype_of(
        assign_expr_type, 
        *identifier_type
    );

    if (!does_assign_conform_declared) {
        classtable->semant_error(this) 
            << "The identifier " 
            << identifier 
            << " has been declared as "
            << *identifier_type
            << " but assigned with incompatible type "
            << assign_expr_type
            << ".\n";

        this->set_type(Object);
        return Object;
    }

    this->set_type(assign_expr_type);
    return assign_expr_type;
}

Symbol branch_class::check_type() {
    Environment->enterscope();
    Environment->addid(name, &type_decl);
    type = expr->check_type();
    Environment->exitscope();
    return type;
}


//=================================================================================
// Semant Main
//=================================================================================

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
    cout << "Constructed classtable" << endl;

    /* some semantic analysis code may go here */
    for(int i = classes->first(); classes->more(i); i = classes->next(i)) {
        type_check(classes->nth(i));
    }
    cout << "Finished typecheck" << endl;

    if (classtable->errors()) {
    cerr << "Compilation halted due to static semantic errors." << endl;
    exit(1);
    }
}
