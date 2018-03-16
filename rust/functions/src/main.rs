fn main() {
    println!("Hello, world!");

    another_function_0();
    another_function_1(3);
    another_function_2(4, 5);

    let y = {
        let x = 3;
        x + 1
    };

    println!("The value of y is: {}", y);


    let x = five();
    println!("The value of x is: {}", x);

    let x = plus_one(5);
    println!("The value of x is: {}", x);

    let mut x = 42;
    side_effect (&mut x);
    println!("After side_effect, x is: {}", x);
}

fn another_function_0() {
    println!("Another function.");
}


fn another_function_1(x: i32) {
    println!("The value of x is: {}", x);
}

fn another_function_2(x: i32, y: i32) {
    println!("The value of x is: {}", x);
    println!("The value of y is: {}", y);
}

fn five() -> i32 {
    5
}

fn plus_one(x: i32) -> i32 {
    x + 1
}

fn side_effect(x:&mut i32) {
    *x = *x + 1;
}
