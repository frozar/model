fn main() {
    example_1();

    example_2();

    example_3();

    example_4();

    example_5();

    example_6();

    example_7();

    example_8();

    example_9();

    example_10();

    example_11();
}

#[derive(Debug)]
enum IpAddrKind {
    V4,
    V6,
}

#[derive(Debug)]
struct IpAddr {
    kind: IpAddrKind,
    address: String,
}

fn example_1() {
    let home = IpAddr {
        kind: IpAddrKind::V4,
        address: String::from("127.0.0.1"),
    };

    let loopback = IpAddr {
        kind: IpAddrKind::V6,
        address: String::from("::1"),
    };
    println!("1 home: {:?}", home);
    println!("1 loopback: {:?}", loopback);
}

#[derive(Debug)]
enum IpAddr_2 {
    V4(String),
    V6(String),
}

fn example_2() {
    let home = IpAddr_2::V4(String::from("127.0.0.1"));
    let loopback = IpAddr_2::V6(String::from("::1"));
    println!("2 home: {:?}", home);
    println!("2 loopback: {:?}", loopback);
}

#[derive(Debug)]
enum IpAddr_3 {
    V4(u8, u8, u8, u8),
    V6(String),
}

fn example_3() {
    let home = IpAddr_3::V4(127, 0, 0, 1);
    let loopback = IpAddr_3::V6(String::from("::1"));
    println!("3 home: {:?}", home);
    println!("3 loopback: {:?}", loopback);
}

enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
    ChangeColor(i32, i32, i32),
}

impl Message {
    fn call(&self) {
        println!("Inside method named 'call' of 'Message' enum");
    }
}

fn example_4() {
    let m = Message::Write(String::from("hello"));
    m.call();
}

fn example_5() {
    let some_number = Some(5);
    let some_string = Some("a string");
    let absent_number: Option<i32> = None;

    println!("some_number  : {:?}", some_number);
    println!("some_string  : {:?}", some_string);
    println!("absent_number: {:?}", absent_number);
}

#[derive(Debug)] // So we can inspect the state in a minute
enum UsState {
    Alabama,
    Alaska,
    // ... etc
}

enum Coin {
    Penny,
    Nickel,
    Dime,
    Quarter(UsState),
}

fn value_in_cents(coin: Coin) -> u32 {
    match coin {
        Coin::Penny => 1,
        Coin::Nickel => 5,
        Coin::Dime => 10,
        Coin::Quarter(state) => {
            println!("State quarter from {:?}!", state);
            25
        },
    }
}

fn example_6() {
    let one_penny   = Coin::Penny;
    let one_nickel  = Coin::Nickel;
    let one_dime    = Coin::Dime;
    let one_quarter = Coin::Quarter(UsState::Alabama);
    value_in_cents(one_penny  );
    value_in_cents(one_nickel );
    value_in_cents(one_dime   );
    value_in_cents(one_quarter);
}

fn plus_one(x: Option<i32>) -> Option<i32> {
    match x {
        None => None,
        Some(i) => Some(i + 1),
    }
}

fn example_7() {
    let five = Some(5);
    let six = plus_one(five);
    let none = plus_one(None);
}

fn example_8() {
    let some_u8_value = 3u8;
    match some_u8_value {
        1 => println!("one"),
        3 => println!("three"),
        _ => (),
    }
}

fn example_9() {
    let some_u8_value = Some(0u8);
    if let Some(3) = some_u8_value {
        println!("three");
    }
}

fn example_10() {
    let mut count = 0;
    let one_quarter = Coin::Quarter(UsState::Alabama);
    match one_quarter {
        Coin::Quarter(state) => println!("State quarter from {:?}!", state),
        _ => count += 1,
    }
}

fn example_11() {
    let mut count = 0;
    let one_quarter = Coin::Quarter(UsState::Alabama);
    if let Coin::Quarter(state) = one_quarter {
        println!("State quarter from {:?}!", state);
    } else {
        count += 1;
    }
}
