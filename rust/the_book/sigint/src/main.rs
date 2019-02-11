fn main() {
    let i0: i8 = -128;
    let i1: i8 = -1;
    println!("i0: {}", i0);
    println!("i1: {}", i1);
    // As 128 is not representable with i8, the following line
    // will triger a main thread panic
    // A signel SIGINT should have been send
    // println!("i0 / i1: {}", i0 / i1);
}
