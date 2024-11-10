class Animal {
  attr name : String;
  attr age : Int;
  
  method eat(food : String) : Object {
    out_string(name + " is eating " + food);
  };
};

class Dog inherits Animal {
  // Attribute with the same name as an attribute in the father class
  attr age : Bool;
  
  method bark() : Object {
    out_string("Woof! Woof!");
  };

  // Method with a call to a non -existent method
  method play() : Object {
    chase_tail();
  };

  // Absent return method
  method sleep() : Object {
    // Code of Method
  };
};

class Main {
  // Main Method with Invalid Parameter
  main(x : String) : Object {
    let dog : Dog <- new Dog;
    dog.bark();
    dog.play();
    dog.sleep();
    dog.eat(true); // Call to the "EAT" method with an invalid argument
  };
};