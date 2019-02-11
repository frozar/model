extern crate add_one;

// As the 'rand' crate is not specified as a dependencies for the
// current crate, "adder", one get a compiling error if one try to
// use it, even if it is a dependencies for the "add-one" crate
// extern crate rand;

fn main() {
    let num = 10;
    println!("Hello, world! {} plus one is {}!", num, add_one::add_one(num));
}

// Run a given crate, for example "adder":
// $ cargo run -p adder
