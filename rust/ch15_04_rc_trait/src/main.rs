fn main() {
    example_01();
    example_02();
    example_03();
}

enum List {
    Cons(i32, Box<List>),
    Nil,
}

use List::{Cons, Nil};

fn example_01() {
    let a = Cons(5,
        Box::new(Cons(10,
            Box::new(Nil))));
    let b = Cons(3, Box::new(a));
    // Not allow to own the list 'a' a second time
    // let c = Cons(4, Box::new(a));
}

enum List_2 {
    Cons(i32, Rc<List_2>),
    Nil,
}

// use List_2::{Cons, Nil};
use std::rc::Rc;

// The call to Rc::clone only increments the reference count (doesn't do a deep copy)
fn example_02() {
    let a = Rc::new(List_2::Cons(5, Rc::new(List_2::Cons(10, Rc::new(List_2::Nil)))));
    let b = List_2::Cons(3, Rc::clone(&a));
    let c = List_2::Cons(4, Rc::clone(&a));
    
    let d = List_2::Cons(42, Rc::clone(&Rc::new(b)));
}

fn example_03() {
    let a = Rc::new(List_2::Cons(5, Rc::new(List_2::Cons(10, Rc::new(List_2::Nil)))));
    println!("count after creating a = {}", Rc::strong_count(&a));
    let b = List_2::Cons(3, Rc::clone(&a));
    println!("count after creating b = {}", Rc::strong_count(&a));
    {
        let c = List_2::Cons(4, Rc::clone(&a));
        println!("count after creating c = {}", Rc::strong_count(&a));
    }
    println!("count after c goes out of scope = {}", Rc::strong_count(&a));
}
