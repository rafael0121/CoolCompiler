
(*
 *  execute "coolc bad.cl" to see the error messages that the coolc parser
 *  generates
 *
 *  execute "myparser bad.cl" to see the error messages that your parser
 *  generates
 *)

(* no error *)
class A {
};

(* error:  b is not a type identifier *)
Class b inherits A {
};

(* error:  a is not a type identifier *)
Class C inherits a {
};

(* error:  keyword inherits is misspelled *)
Class D inherts A {
};

(* error:  closing brace is missing *)
Class E inherits A {
;

(* no error:  due to previous error it skips the next class as long as it is correct*)
Class E2 inherits A {
};

(* error: feature cannot start with uppercase*)
Class F inherits A {
    F_val1 : Int;
    f_val2 : String <- 9;
    f_val4 : Int <- "WRONG";
};

(* error: Let expression must be inside a feature.*)
Class G inherits A {
    LET g_val1 : String, g_val2 : Int IN {
        g_val2 <- 1;
    };
    g_val1 <- "TEST";
};
