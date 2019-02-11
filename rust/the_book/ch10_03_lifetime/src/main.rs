fn main (){
    example_1();
    example_2();
    // example_3();
    example_4();
    example_5();
}

fn example_1(){
    let string1 = String::from("abcd");
    let string2 = "xyz";

    let result = longest(string1.as_str(), string2);
    println!("The longest string is {}", result);
}

// Example of syntax for lifetime expression
// &i32        // a reference
// &'a i32     // a reference with an explicit lifetime
// &'a mut i32 // a mutable reference with an explicit lifetime

fn longest<'a>(x: &'a str, y: &'a str) -> &'a str {
    if x.len() > y.len() {
        x
    } else {
        y
    }
}

fn example_2(){
    let string1 = String::from("long string is long");

    {
        let string2 = String::from("xyz");
        let result = longest(string1.as_str(), string2.as_str());
        println!("The longest string is {}", result);
    }
}

// fn example_3(){
//     let string1 = String::from("long string is long");
//     let result;
//     {
//         let string2 = String::from("xyz");
//         result = longest(string1.as_str(), string2.as_str());
//     }
//     println!("The longest string is {}", result);
// }
// error[E0597]: `string2` does not live long enough
//   --> src/main.rs:43:44
//    |
// 43 |         result = longest(string1.as_str(), string2.as_str());
//    |                                            ^^^^^^^ borrowed value does not live long enough
// 44 |     }
//    |     - `string2` dropped here while still borrowed
// 45 |     println!("The longest string is {}", result);
// 46 | }
//    | - borrowed value needs to live until here

fn longest_2<'a>(x: &'a str, y: &str) -> &'a str {
    x
}

fn example_4(){
    let string1 = String::from("abcd");
    let string2 = "xyz";

    let result = longest_2(string1.as_str(), string2);
    println!("The longest string is {}", result);
}

// fn longest_3<'a>(x: &str, y: &str) -> &'a str {
//     let result = String::from("really long string");
//     result.as_str()
// }
// error[E0597]: `result` does not live long enough
//   --> src/main.rs:73:5
//    |
// 73 |     result.as_str()
//    |     ^^^^^^ borrowed value does not live long enough
// 74 | }
//    | - borrowed value only lives until here
//    |
// note: borrowed value must be valid for the lifetime 'a as defined on the function body at 71:1...
//   --> src/main.rs:71:1
//    |
// 71 | / fn longest_3<'a>(x: &str, y: &str) -> &'a str {
// 72 | |     let result = String::from("really long string");
// 73 | |     result.as_str()
// 74 | | }
//    | |_^

struct ImportantExcerpt<'a> {
    part: &'a str,
}

// An object which instanciate ImportantExcerpt will live as long as the
// reference it contains in 'part' field
fn example_5(){
    let novel = String::from("Call me Ishmael. Some years ago...");
    let first_sentence = novel.split('.')
        .next()
        .expect("Could not find a '.'");
    let i = ImportantExcerpt { part: first_sentence };
}

// The 3 Lifetime elision rules
// Rule 1: each parameter that is a reference gets its own lifetime parameter
//   Example: fn foo(x: &i32, y: &i32)  =>  fn foo<'a, 'b>(x: &'a i32, y: &'b i32)

// Rule 2: if there is exactly one input lifetime parameter, that lifetime is
//         assigned to all output lifetime parameters
//   Example: fn foo(x: &i32) -> &i32  =>  fn foo<'a>(x: &'a i32) -> &'a i32

// Rule 3: if there are multiple input lifetime parameters, but one of
//         them is '&self' or '&mut self' because this is a method, the lifetime
//         of self is assigned to all output lifetime parameters
//   Example: fn foo(&self, x: &i32) -> &i32 => fn foo<'a, 'b>(&'a self, x: &'b i32) -> &'a i32
// (Rule 1 and 3 are apply above)

impl<'a> ImportantExcerpt<'a> {
    fn level(&self) -> i32 {
        3
    }

    // Don't to precision lifetime information in this signature because
    // Lifetime elision rule 1 and 3 apply
    fn announce_and_return_part(&self, announcement: &str) -> &str {
        println!("Attention please: {}", announcement);
        self.part
    }
}

// It is also possible to use a 'static lifetime, which mean during all the
// program execution
// let s: &'static str = "I have a static lifetime.";

use std::fmt::Display;

fn longest_with_an_announcement<'a, T>(x: &'a str, y: &'a str, ann: T) -> &'a str
    where T: Display
{
    println!("Announcement! {}", ann);
    if x.len() > y.len() {
        x
    } else {
        y
    }
}

