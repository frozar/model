fn main() {
    example_01();
    example_02();
    example_03();
    example_04();
    example_05();
}

fn example_01() {
    let x = 5;
    let y = &x;

    assert_eq!(5, x);
    assert_eq!(5, *y);
}

fn example_02() {
    let x = 5;
    let y = Box::new(x);

    assert_eq!(5, x);
    assert_eq!(5, *y);
}

struct MyBox<T>(T);

impl<T> MyBox<T> {
    fn new(x: T) -> MyBox<T> {
        MyBox(x)
    }
}

fn example_03() {
    let x = 5;
    let y = Box::new(x);

    assert_eq!(5, x);
    assert_eq!(5, *y);
}

use std::ops::Deref;

impl<T> Deref for MyBox<T> {
    type Target = T;

    fn deref(&self) -> &T {
        &self.0
    }
}

fn hello(name: &str) {
    println!("Hello, {}!", name);
}

fn example_04() {
    let m = MyBox::new(String::from("Rust"));
    hello(&m);
}

// If the Deref coercions doesn't exist, one should have wrote
// something like in the following function
fn example_05() {
    let m = MyBox::new(String::from("Rust"));
    hello(&(*m)[..]);
}
