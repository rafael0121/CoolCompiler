(*
 *  CS164 Fall 94
 *
 *  Programming Assignment 1
 *    Implementation of a simple stack machine.
 *
 *  Skeleton file
 *)

class Cell {
   charCell: String;
   cellBellow: Cell;

   (* Initializes normal cells with a char and the cell currentlly on top *)
   init(i: String, j: Cell): Cell{  
      {
      charCell <- i;
      cellBellow <- j;
      self;
      }  
   };

   (* Initializes sentinell cell, with a special character and pointing to void *)
   initSent(i: String): Cell{  
      {
      charCell <- i;
      self;
      }  
   };

   getCellBellow(): Cell { cellBellow };

   getChar(): String { charCell };
};

class Stack inherits Object{
   stackTop: Cell;

   (*Initializes stack with a sentinel cell, afterwards the stack may be filled*)
   initStack(): Stack{
      {
      stackTop <- (new Cell).initSent("#");
      self;
      }    
   };

   (* Removes character from the pile, and updates the top *)
   popStack(): Stack{
      {
      stackTop <- stackTop.getCellBellow();
      self;
      }
   };

   (* Adds new character to the pile, and updates the top *)
   pushStack(i : String): Stack {
      {
         stackTop <- (new Cell).init(i, stackTop);
         self;
      }
   };

   (* Function to clear the Stack at the end of the programs execution *)
   clearStack(): Stack{
      {
         while(not isvoid stackTop) loop{
            stackTop <- stackTop.getCellBellow();
         }
         pool;
         self;
      }
   };

   processEntry(): Object{
      let auxChar1: String,
         auxChar2: String,
         auxCell: Cell,
         auxRes: Int,
         converter: A2I <- new A2I,
         inOut: IO <- new IO
      in{
         (* Executes sum of the two first integers of the pile *)
         if stackTop.getChar() = "+" then
         {
            popStack();

            auxChar1 <- stackTop.getChar();
            popStack();
            auxChar2 <- stackTop.getChar();
            popStack();

            auxRes <- converter.a2i(auxChar1) + converter.a2i(auxChar2);
            pushStack(converter.i2a(auxRes));
         }
         (* Swaps the two first cells of the pile *)
         else if stackTop.getChar() = "s" then
         {
            popStack();

            auxChar1 <- stackTop.getChar();
            popStack();
            auxChar2 <- stackTop.getChar();
            popStack();

            pushStack(auxChar1);
            pushStack(auxChar2);
         }
         (* Prints the pile's content *)
         else if stackTop.getChar() = "d" then
         {
            popStack();

            auxCell <- stackTop;
            while not(auxCell.getChar() = "#")loop
            {
               inOut.out_string(auxCell.getChar().concat("\n"));
               auxCell <- auxCell.getCellBellow();
            }
            pool;
         } else self
         fi
         fi
         fi;
      }
   };
};


class Main inherits IO {
   run : Bool <- true;
   pilha : Stack <- (new Stack).initStack();

   main() : Object {
      let entry : String
      in {
         (* Main loop, receives input and runs until "x" is received *)
         while(run) loop
         {
            out_string(">");
            entry <- in_string();

            if(entry = "e") then{
               pilha.processEntry();
            }
            else if(entry = "x") then{
               pilha.clearStack();
               run <- false;
               out_string("Pilha esvaziada! Encerrando programa \n");
            }
            else if(entry = "d") then{
               (* Didn't want to fix the processEntry for the 'd' case, this was the easiest solution *)
               pilha.pushStack(entry);
               pilha.processEntry();
            }
            else{
               (* Guaranteed to be a number, "s" or "+" *)
               pilha.pushStack(entry);
            }
            fi fi fi;
         }
         pool;
      }
   };
};
