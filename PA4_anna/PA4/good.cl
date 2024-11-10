(*
   The class A2I provides integer-to-string and string-to-integer
conversion routines.  To use these routines, either inherit them
in the class where needed, have a dummy variable bound to
something of type A2I, or simpl write (new A2I).method(argument).
*)


(*
   c2i   Converts a 1-character string to an integer.  Aborts
         if the string is not "0" through "9"
*)
class A2I {

     c2i(char : String) : Int {
	if char = "0" then 0 else
	if char = "1" then 1 else
	if char = "2" then 2 else
        if char = "3" then 3 else
        if char = "4" then 4 else
        if char = "5" then 5 else
        if char = "6" then 6 else
        if char = "7" then 7 else
        if char = "8" then 8 else
        if char = "9" then 9 else
        { abort(); 0; }  -- the 0 is needed to satisfy the typchecker
        fi fi fi fi fi fi fi fi fi fi
     };

(*
   i2c is the inverse of c2i.
*)
     i2c(i : Int) : String {
	if i = 0 then "0" else
	if i = 1 then "1" else
	if i = 2 then "2" else
	if i = 3 then "3" else
	if i = 4 then "4" else
	if i = 5 then "5" else
	if i = 6 then "6" else
	if i = 7 then "7" else
	if i = 8 then "8" else
	if i = 9 then "9" else
	{ abort(); ""; }  -- the "" is needed to satisfy the typchecker
        fi fi fi fi fi fi fi fi fi fi
     };

(*
   a2i converts an ASCII string into an integer.  The empty string
is converted to 0.  Signed and unsigned strings are handled.  The
method aborts if the string does not represent an integer.  Very
long strings of digits produce strange answers because of arithmetic 
overflow.

*)
     a2i(s : String) : Int {
        if s.length() = 0 then 0 else
	if s.substr(0,1) = "-" then ~a2i_aux(s.substr(1,s.length()-1)) else
        if s.substr(0,1) = "+" then a2i_aux(s.substr(1,s.length()-1)) else
           a2i_aux(s)
        fi fi fi
     };

(*
  a2i_aux converts the usigned portion of the string.  As a programming
example, this method is written iteratively.
*)
     a2i_aux(s : String) : Int {
	(let int : Int <- 0 in	
           {	
               (let j : Int <- s.length() in
	          (let i : Int <- 0 in
		    while i < j loop
			{
			    int <- int * 10 + c2i(s.substr(i,1));
			    i <- i + 1;
			}
		    pool
		  )
	       );
              int;
	    }
        )
     };

(*
    i2a converts an integer to a string.  Positive and negative 
numbers are handled correctly.  
*)
    i2a(i : Int) : String {
	if i = 0 then "0" else 
        if 0 < i then i2a_aux(i) else
          "-".concat(i2a_aux(i * ~1)) 
        fi fi
    };
	
(*
    i2a_aux is an example using recursion.
*)		
    i2a_aux(i : Int) : String {
        if i = 0 then "" else 
	    (let next : Int <- i / 10 in
		i2a_aux(next).concat(i2c(i - next * 10))
	    )
        fi
    };

};

(*
 *  CS164 Fall 94
 *
 *  Programming Assignment 1
 *    Implementation of a simple stack machine.
 *
 *  Skeleton file
 *)

-- Class that implements a character stack
class Stack {
  -- Stack content (I used "string" because I still don't know how to use array)
  content : String;

  -- Get stack content
  getContent() : String {{
    content;
  }};

  -- Stacks an element
  push(x : String) : Stack {{
    content <- content.concat(x);
    self;
  }};

  -- Unstack an element
  pop() : Stack {{
    let i : Int <- content.length() in
    {
      if i = 0 then
        ~1
      else {
        content <- content.substr(0, i - 1);
        i - 1;
      } fi;
    };
    self;
  }};
};

-- Main class
class Main inherits IO {
  -- Instance of classes
  a2i : A2I <- new A2I;
  stack : Stack <- new Stack;

  -- IO
  prompt() : String {{
     out_string(">");
     in_string();
  }};

  -- Display stack contents from top to bottom
  showStackContent(stackContent : String) : String {{
    let i : Int <- 0 in
    let j : Int <- stackContent.length() in
    {
      while i < j loop
        {
          out_string(stackContent.substr(j-i-1, 1).concat("\n"));
          i <- i + 1;
        }
      pool;
    };
    "";
  }};

  -- Analyzes stack content
  analyzesStackContent() : String {{
    if (stack.getContent().length() = 0) then
      ""
    else {
      -- Fetch the top element of the stack
      let s : String <- stack.getContent().substr(stack.getContent().length() - 1, 1) in
      {
        if s = "+" then {
          -- Remove the "+" symbol
          stack.pop();

          -- Fetch the top two elements to be sum
          let a : Int <- a2i.a2i(stack.getContent().substr(stack.getContent().length() - 1, 1)) in
          let b : Int <- a2i.a2i(stack.getContent().substr(stack.getContent().length() - 2, 1)) in
          {
            -- Removes the top two elements
            stack.pop(); 
            stack.pop();

            -- Stack the sum result
            stack.push(a2i.i2a(a + b));
          };
        }

        else if s = "s" then {
          -- Remove the "s" character
          stack.pop();

          -- Fetch the top two elements to be swapped
          let a : String <- stack.getContent().substr(stack.getContent().length() - 1, 1) in
          let b : String <- stack.getContent().substr(stack.getContent().length() - 2, 1) in
          {
            -- Removes the top two elements
            stack.pop(); 
            stack.pop();

            -- Stacks the elements in reversed order
            stack.push(a);
            stack.push(b);
          };
        }

        else ""

        fi fi;
      };
    } fi;
    "";
  }};

  -- Checks if the string is an integer
  isInteger(input : String) : Bool {
    if a2i.a2i(input) = ~1 then
      false
    else
      true
    fi
  };

  -- Process user input
  process(s : String) : String {{
    -- Displays stack content
    if s = "d" then
      showStackContent(stack.getContent())

    -- Analyzes stack content
    else if s = "e" then
      analyzesStackContent()

    -- Stacks the "s" character
    else if s = "s" then
      stack <- stack.push(s)
    
    -- Stacks the integer (or "+" symbol)
    else if isInteger(s) then
      stack <- stack.push(s)

    -- Unknown command
    else
      out_string("Unknown command: ".concat(s).concat("\n"))

    fi fi fi fi;
    "";
  }};

  -- Main method
  main(): Object {{
    -- Loop control variable
    let running : Bool <- true in
    {
      while running loop 
        (
          let s : String <- prompt() in
            -- Closes the program
            if s = "x" then
              running <- false

            -- Process user input
            else 
              process(s)
            fi
      )
      pool;
    };
  }};
};
