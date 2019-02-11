fn main() {
    example_01();
    // example_02();
    example_03();
}

// Box: allocate data on the heap
fn example_01() {
    let b = Box::new(5);
    println!("b = {}", b);
}

// The example_02 doesn't compile because of the recursive 'List' type
// enum List {
//     Cons(i32, List),
//     Nil,
// }

// use List::{Cons, Nil};

// fn example_02() {
//     let list = Cons(1, Cons(2, Cons(3, Nil)));
// }

// Recursive type
// Use the 'Box' type to allow the compile to get the
// size of an element of this type
enum List {
    Cons(i32, Box<List>),
    Nil,
}

use List::{Cons, Nil};

fn example_03() {
    let list = Cons(1,
        Box::new(Cons(2,
            Box::new(Cons(3,
                Box::new(Nil))))));
}
