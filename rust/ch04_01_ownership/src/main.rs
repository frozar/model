fn main() {
    let s1 = String::from("hello");
    let s2 = s1;

    // println!("{}, world!", s1);
    // The above line would not compile because "let _s2 = s1;" does a move.
    // This means all use of s1 after this line will result in an error
    // because "s1" does exist any more in this scope. Compiler error:
    // error[E0382]: use of moved value: `s1`
    //  --> src/main.rs:5:28
    //   |
    // 3 |     let _s2 = s1;
    //   |         --- value moved here
    // 4 | 
    // 5 |     println!("{}, world!", s1);
    //   |                            ^^ value used here after move
    //   |
    //   = note: move occurs because `s1` has type `std::string::String`, which does not implement the `Copy` trait

    // An explicit call to "clone" method do a deep copy of the data,
    // which alleviate the ownership matter
    let s1 = String::from("hello");
    let s2 = s1.clone();

    println!("s1 = {}, s2 = {}", s1, s2);

    // With primitif type, as the data is store in the stack, the
    // problem doesn't appear
    let x = 5;
    let y = x;

    println!("x = {}, y = {}", x, y);

    example_1();

    example_2();

    example_3();

    example_4();

    example_5();

    example_6();

    example_7();
}

fn example_1() {
    let s = String::from("hello");  // s comes into scope.

    takes_ownership(s);             // s's value moves into the function...
                                    // ... and so is no longer valid here.

    let x = 5;                      // x comes into scope.

    makes_copy(x);                  // x would move into the function,
                                    // but i32 is Copy, so it’s okay to still
                                    // use x afterward.
} // Here, x goes out of scope, then s. But since s's value was moved, nothing
  // special happens.

fn takes_ownership(some_string: String) { // some_string comes into scope.
    println!("{}", some_string);
} // Here, some_string goes out of scope and `drop` is called. The backing
  // memory is freed.

fn makes_copy(some_integer: i32) { // some_integer comes into scope.
    println!("{}", some_integer);
} // Here, some_integer goes out of scope. Nothing special happens.

fn example_2() {
    let s1 = gives_ownership();         // gives_ownership moves its return
                                        // value into s1.

    let s2 = String::from("hello");     // s2 comes into scope.

    let s3 = takes_and_gives_back(s2);  // s2 is moved into
                                        // takes_and_gives_back, which also
                                        // moves its return value into s3.
} // Here, s3 goes out of scope and is dropped. s2 goes out of scope but was
  // moved, so nothing happens. s1 goes out of scope and is dropped.


fn gives_ownership() -> String {             // gives_ownership will move its
                                             // return value into the function
                                             // that calls it.

    let some_string = String::from("hello"); // some_string comes into scope.

    some_string                              // some_string is returned and
                                             // moves out to the calling
                                             // function.
}

// takes_and_gives_back will take a String and return one.
fn takes_and_gives_back(a_string: String) -> String { // a_string comes into
                                                      // scope.

    a_string  // a_string is returned and moves out to the calling function.
}

// Handle many return value for a single function
// The issue with the tuple code at the end of the preceding section is
// that we have to return the String to the calling function so we can
// still use the String after the call to calculate_length, because the
// String was moved into calculate_length.
fn example_3() {
    let s1 = String::from("hello");

    let (s2, len) = calculate_length(s1);

    println!("The length of '{}' is {}.", s2, len);
}

fn calculate_length(s: String) -> (String, usize) {
    let length = s.len(); // len() returns the length of a String.

    (s, length)
}

// Use a reference prototype to avoid to lose the ownership of the
// String s1
fn example_4() {
    let s1 = String::from("hello");

    let len = calculate_length_2(&s1);

    println!("The length of '{}' is {}.", s1, len);
}

fn calculate_length_2(s: &String) -> usize { // s is a reference to a String
    s.len()
} // Here, s goes out of scope. But because it does not have ownership of what
  // it refers to, nothing happens.

fn example_5() {
    let mut s = String::from("hello");

    change(&mut s);
}

fn change(some_string: &mut String) {
    some_string.push_str(", world");
}

fn example_6() {
    let mut s = String::from("hello");

    // This code won't compile: only one mutable reference is allow by Rust
    // let r1 = &mut s;
    // let r2 = &mut s;

    // This is possible
    {
        let r1 = &mut s;

    } // r1 goes out of scope here, so we can make a new reference with no problems.

    let r2 = &mut s;

    // This code is also not possible. You cannot have a mutable reference toward
    // a variable which is referenced as an immutable reference somewhere else.
    // let r1 = &s; // no problem
    // let r2 = &s; // no problem
    // let r3 = &mut s; // BIG PROBLEM
}

fn example_7() {
    let my_string = String::from("hello world");

    // first_word works on slices of `String`s
    let word = first_word(&my_string[..]);

    let my_string_literal = "hello world";

    // first_word works on slices of string literals
    let word = first_word(&my_string_literal[..]);

    // since string literals *are* string slices already,
    // this works too, without the slice syntax!
    let word = first_word(my_string_literal);

    println!("First word: {}", word);
}

fn first_word(s: &str) -> &str {
    let bytes = s.as_bytes();

    for (i, &item) in bytes.iter().enumerate() {
        if item == b' ' {
            return &s[0..i];
        }
    }

    &s[..]
}
