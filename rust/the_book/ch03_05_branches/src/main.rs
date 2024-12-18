fn main() {
    // IF
    let number = 3;

    if number < 5 {
        println!("condition was true");
    } else {
        println!("condition was false");
    }

    if number != 0 {
        println!("number was something other than zero");
    }


    let number = 6;
    if number % 4 == 0 {
        println!("number is divisible by 4");
    } else if number % 3 == 0 {
        println!("number is divisible by 3");
    } else if number % 2 == 0 {
        println!("number is divisible by 2");
    } else {
        println!("number is not divisible by 4, 3, or 2");
    }


    // Ternary operation
    let condition = true;
    let number = if condition {
        5
    } else {
        6
    };

    println!("The value of number is: {}", number);

    // FOR
    // Inifinite loop
    // loop {
    //     println!("again, and again!");
    // }
    
    // WHILE
    let mut number = 3;

    while number != 0 {
        println!("{}!", number);

        number = number - 1;
    }

    println!("LIFTOFF!!!");

    // LOOP
    let a = [10, 20, 30, 40, 50];
    let mut index = 0;

    while index < 5 {
        println!("the value is: {}", a[index]);

        index = index + 1;
    }
    println!("");

    
    for element in a.iter() {
        println!("the value is: {}", element);
    }

    
    for number in (1..4).rev() {
        println!("{}!", number);
    }
    println!("LIFTOFF!!!");

    for number in 1..4 {
        println!("{}!", number);
    }
    println!("LIFTOFF!!!");

    // Currently experimental
    // for number in (1..4).step_by(2) {
    //     println!("{}!", number);
    // }
    // println!("LIFTOFF!!!");
}
