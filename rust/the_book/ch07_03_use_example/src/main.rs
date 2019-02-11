fn main() {
    example_1();
    example_2();
    example_3();
}

pub mod a {
    pub mod series {
        pub mod of {
            pub fn nested_modules() {
                println!("In nested_modules");
            }
        }
    }
}

use a::series::of;

fn example_1() {
    of::nested_modules();
}

use a::series::of::nested_modules;

fn example_2() {
    nested_modules();
}

#[derive(Debug)]
enum TrafficLight {
    Red,
    Yellow,
    Green,
}

use TrafficLight::{Red, Yellow};

fn example_3() {
    let red = Red;
    let yellow = Yellow;
    let green = TrafficLight::Green;
    println!("red   : {:?}", red);
    println!("yellow: {:?}", yellow);
    println!("green : {:?}", green);
}

#[derive(Debug)]
enum TrafficLight_2 {
    Red,
    Yellow,
    Green,
}

use TrafficLight_2::*;

fn example_4() {
    let red = Red;
    let yellow = Yellow;
    let green = Green;
    println!("red   : {:?}", red);
    println!("yellow: {:?}", yellow);
    println!("green : {:?}", green);
}
