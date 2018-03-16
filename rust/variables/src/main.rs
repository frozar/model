fn main() {
    // 1st example
    let mut x = 5;
    println!("The value of x is: {}", x);
    x = 6;
    println!("The value of x is: {}", x);

    // 2nd example
    let x = 5;

    let x = x + 1;

    let x = x * 2;

    println!("The value of x is: {}", x);

    // 3rd example
    let spaces = "   ";
    let spaces = spaces.len();

    // 4th example
    let mut spaces = "   ";
    // This foolowing line will not compile:
    // a mutable variable cannot change its type during its life
    // spaces = spaces.len();
}
