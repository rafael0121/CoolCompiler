
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)

class Main {
  main():Int { 0 };
};

class Grandparent {
  first : Object;
};

class Parent inherits Grandparent {
  second : Object;
};

class Child inherits Parent {
  third : Object;
  fourth : Object;
};
