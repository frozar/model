use std::io;

macro_rules! parse_input {
    ($x:expr, $t:ident) => {
        $x.trim().parse::<$t>().unwrap()
    };
}

fn parse_input() -> (String, u32, Vec<String>, String) {
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let operation = input_line.trim_matches('\n').to_string();
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let pseudo_random_number = parse_input!(input_line, u32);
    let mut rotors: Vec<String> = vec![];
    for i in 0..3 as usize {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        let rotor = input_line.trim_matches('\n').to_string();
        rotors.push(rotor);
    }
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let message = input_line.trim_matches('\n').to_string();

    (operation, pseudo_random_number, rotors, message)
    // ()
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
fn main() {
    let (operation, shift, rotors, message) = parse_input();

    eprintln!("{:?} {:?} {:?} {:?}", operation, shift, rotors, message);

    let shifted_message: String = message
        .chars()
        .enumerate()
        .map(|(i, c)| std::char::from_u32(c as u32 + shift as u32 + i as u32).unwrap_or(c))
        .collect();
    eprintln!("shifted_message {:?}", shifted_message);

    println!("message");
}
