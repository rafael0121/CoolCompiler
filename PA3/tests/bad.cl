
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

(* error: feature cannot start with uppercase and receives different type *)
Class F inherits A {
    f_val1 : Int <- 7;
    f_val2 : String <- 9;
    f_val4 : Int <- "WRONG";
};

(* error: LET variable link and usage outside block *)
Class G inherits A {
    LET g_val1 : String, g_val2 : Int IN {
        g_val2 <- 1;
    }
    g_val1 <- "TEST";

};