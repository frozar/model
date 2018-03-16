
// Disable warning about unused variable for a function
#[allow(unused_variables, unused_mut)]
fn main() {
    // Integer, default: i32
    let i_i8   : i8    = -42;
    let i_u8   : u8    =  42;
    let i_i16  : i16   = -42;
    let i_u16  : u16   =  42;
    let i_i32  : i32   = -42;
    let i_u32  : u32   =  42;
    let i_i64  : i64   = -42;
    let i_u64  : u64   =  42;
    let i_isize: isize = -42;
    let i_usize: usize =  42;

    // Literal integer
    let i0 = 42_424;      // decimal
    let i0 = 0xff;        // hexadecimal
    let i0 = 0o77;        // octal
    let i0 = 0b1111_0000; // binary
    let i0 = b'A';        // byte (like u8)

    // Float, default: f64
    let x: f32 = 42.42;
    let x: f64 = 2.2;

    // Numerical operation
    // addition
    let sum = 5 + 10;
    // subtraction
    let difference = 95.5 - 4.3;
    // multiplication
    let product = 4 * 30;
    // division
    let quotient = 56.7 / 32.2;
    // remainder
    let remainder = 43 % 5;

    // Boolean
    let t       = true;
    let f: bool = false; // with explicit type annotation

    // Character (that's amazing)
    let c = 'z';
    let z = 'ℤ';
    let heart_eyed_cat = '😻';  // Should be the face of a smiling cat

    // Tuple
    let tup: (i32, f64, u8) = (500, 6.4, 1);
    let (x, y, z) = tup;
    println!("The value of y is: {}", y);
    let five_hundred   = tup.0;
    let six_point_four = tup.1;
    let one            = tup.2;

    // Array
    let a = [1, 2, 3, 4, 5];
    let first  = a[0];
    let second = a[1];

    // Array of tuple
    let combine = [(0.0, 0.1), (1.0, 1.1)];
    println!("Combine data structure: {}", combine[0].1);
}
