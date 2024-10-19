(*
 *  CS164 Fall 94
 *
 *  Programming Assignment 1
 *    Implementation of a simple stack machine.
 *
 *  Skeleton file
 *)

class Main inherits IO {

    (* Pushdown Automata *)
    pushdown: Stack <- (new Stack).init();

    (* User Input *)
    input: UserInput <- new UserInput;

    main() : Object {
        {
            (* Run intepreter. *)
            out_string(">");
            while not { { input } = "x"; } loop
                let input_char: UserInput <- new UserInput in {
                    input_char.setchar(input.digit());
                        if input_char.digit() = "+" then pushdown.push(input_char) else
                        if input_char.digit() = "s" then pushdown.push(input_char) else
                        if input_char.digit() = "e" then pushdown.e() else
                        if input_char.digit() = "d" then pushdown.d() else { 
                            pushdown.push(input_char); 
                        }
                            fi fi fi fi;
                    out_string(">");
                }
            pool;

            (* Exit string. *)
            out_string("\n Ja vai? E o Cafezin? (^_^) c[] \n");
        }
    };
};

(*
 * Class UserInput: Handles user's input.
 * Inherits: IO
 *)

class UserInput inherits IO {

    (* Stores User Input *)
    digit: String;

    (* Return stored input *)
    digit(): String { digit };

    (* Gets a new User's input from stdin. *)
    getchar(): String {
        let input: String <- in_string() in {
            digit <- input;
        }
    };

    (* Stores a new digit passed by parameter. *)
    setchar(new_digit: String): SELF_TYPE { { digit <- new_digit; self; } };

    (* Cast digit "string" to Class Int, abort if can't *)
    toInt(): Int {
        (new A2I).a2i(digit)
    };
};

(*
 * Class Stack: Handles stack storage and commands.
 * Inherits: UserInput.
 *)
class Stack inherits UserInput {
    
    (* Stores the node's object. *)
    obj: UserInput <- New UserInput;
    
    (* References the next node. *)
    next: Stack;

    (* Inits stack. *)
    init(): SELF_TYPE { { obj.setchar("empty"); self;} }; 

    (* Return next node. *)
    next(): Stack { next };

    (* Return stored object. *)
    obj(): UserInput { obj };

    (* Set new value for obj. *)
    setobj(new_obj: UserInput): Object { obj <- new_obj };

    (* Set new referenced next node *)
    setnext(new_next: Stack): Stack { next <- new_next };

    (* Command e, handles stack based in top element. *)
    e(): Object {
            (* stack: s a b => stack: b a *)
            if obj.digit() = "s" then {
                self.pop(); -- pop "s"
                if isvoid next then self
                else
                    let a: UserInput <- obj() in
                    let b: UserInput <- next.obj() in {
                        next.setobj(a);
                        self.setobj(b);
                    }
                fi;
            }
            else
            (* stack: + n1 n2 => stack: (n1 + n2) *)
            if obj.digit() = "+" then {
                self.pop(); -- pop "+"
                let n1: Int <- obj().toInt() i
                let n2: Int <- next.obj().toInt() in {
                    self.pop(); -- pop "n1"
                    self.pop(); -- pop "n2"
                    self.push((New UserInput).setchar((new A2I).i2a(n1 + n2)));
                };
            } else self fi 
            fi
    };

    (* Command d, print stack *)
    d(): Object {
        if isvoid next then 
            self
        else {
            out_string(obj.digit());
            out_string("\n");
            next.d();
        } fi
    };

    (* Push a element on the top of stack *)
    push(input: UserInput): SELF_TYPE {
        let new_stack: Stack <- new Stack in {
            new_stack.setobj(obj); 
            new_stack.setnext(next); 
            next <- new_stack;
            obj <- input;
            self;
        }
    };

    (* Pop a element from the top of stack *)
    pop(): UserInput {
        if (isvoid next) n { self; }
        else {
                let pop_obj: UserInput <- obj in {
                    self.setobj(next.obj());
                    self.setnext(next.next());
                    pop_obj;
                };
        }
        fi
    };
};
