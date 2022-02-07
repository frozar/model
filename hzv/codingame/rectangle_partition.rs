use std::io;

macro_rules! parse_input {
    ($x:expr, $t:ident) => {
        $x.trim().parse::<$t>().unwrap()
    };
}

fn parse_input() -> (i32, i32, Vec<i32>, Vec<i32>) {
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let inputs = input_line.split(" ").collect::<Vec<_>>();
    let w = parse_input!(inputs[0], i32);
    let h = parse_input!(inputs[1], i32);
    let count_x = parse_input!(inputs[2], i32);
    let count_y = parse_input!(inputs[3], i32);
    let mut inputs = String::new();
    io::stdin().read_line(&mut inputs).unwrap();
    let mut x_separator = vec![];
    for i in inputs.split_whitespace() {
        let x = parse_input!(i, i32);
        x_separator.push(x);
    }
    let mut y_separator = vec![];
    let mut inputs = String::new();
    io::stdin().read_line(&mut inputs).unwrap();
    for i in inputs.split_whitespace() {
        let y = parse_input!(i, i32);
        y_separator.push(y);
    }
    (w, h, x_separator, y_separator)
}

fn computer_length(separators: Vec<i32>, total_length: i32) -> Vec<i32> {
    let mut length = vec![];

    for i in 0..separators.len() {
        length.push(separators[i]);
        for j in (i + 1)..separators.len() {
            length.push(separators[j] - separators[i]);
        }
        length.push(total_length - separators[i]);
    }
    length.push(total_length);

    length
}

fn count_item<T: std::cmp::PartialEq>(items: &Vec<T>, item: T) -> i32 {
    let mut res = 0;
    for one_item in items {
        if (*one_item == item) {
            res += 1;
        }
    }
    res
}

fn main() {
    let (width, height, x_separator, y_separator) = parse_input();

    // eprintln!(
    //     "{:?} {:?} {:?} {:?}",
    //     width, height, x_separator, y_separator
    // );

    let mut x_length = computer_length(x_separator, width);
    let mut y_length = computer_length(y_separator, height);

    // eprintln!("x_length {:?}", x_length);
    // eprintln!("y_length {:?}", y_length);

    let mut total = 0;
    for len in x_length {
        total += count_item(&y_length, len);
    }

    println!("{}", total);
}
