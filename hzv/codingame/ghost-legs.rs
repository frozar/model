use std::io;
use std::collections::HashMap;
use std::fmt;

macro_rules! parse_input {
    ($x:expr, $t:ident) => ($x.trim().parse::<$t>().unwrap())
}

// fn ComputeAnswer(nb_line: usize, connections: Vec<Vec<(usize, usize)>>) -> Vec<usize> {
fn ComputeAnswer(nb_line: usize, connections: Vec<Vec<Connexion>>) -> Vec<usize> {
    let mut answer: Vec<usize> = Vec::new();

    let verbose_idx = 1000;

    for input_idx in 0..nb_line {
        if input_idx == verbose_idx {
            eprintln!("input_idx {:?}", input_idx);
        }
        
        // let mut current_line_idx = input_idx as i32;
        let mut current_line_idx = input_idx;
        
        let mut current_depth = 0;
        loop {
            if let Some(connexion) = connections[current_line_idx]
            // if let Some(connexion) = connections[current_line_idx as usize]
                .iter()
                // .filter_map(|t| 
                //             if current_depth <= t.0 {
                //                 Some(t)
                //             }
                //             else {
                //                 None
                //             }
                .filter_map(|connexion| 
                            if current_depth <= connexion.depth {
                                Some(connexion)
                            }
                            else {
                                None
                            }
                ).next() {
                    // let (next_depth, next_line) = t.to_tuple();
                    let (next_depth, next_line) = connexion.to_tuple();
                    current_depth    = next_depth + 1;
                    // current_line_idx = *next_line;
                    current_line_idx = next_line;
                }
            else
            {
                answer.push(current_line_idx as usize);
                break;
            };

            if input_idx == verbose_idx {
                eprintln!("current_line_idx {:?}", current_line_idx);
                eprintln!("working answer {:?}", answer);
            }
        }
    }

    return answer;
}

fn DBG_P(line: &str) {
    let verbose = false;
    if verbose {
        eprintln!("{}", line);
    }
}

fn ParseInput() -> InputData {

    let mut inputData = InputData::new();

    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let inputs = input_line.split(" ").collect::<Vec<_>>();

    inputData.w = parse_input!(inputs[0], i32);
    inputData.h = parse_input!(inputs[1], i32);
    
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    DBG_P(input_line.trim_right());

    inputData.input_labels = input_line.trim_right().split_whitespace().map(|s| s.to_string()).collect();

    for i in 0..inputData.input_labels.len() {
        inputData.connections.push(Vec::new());
    }

    for i in 1..(inputData.h-1) as usize {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        let line = input_line.trim_right().to_string();
        DBG_P(&line);

        for ii in 0..(inputData.input_labels.len() - 1) {
            let i_start = 3 * ii + 1;
            let i_end = 3 * ii + 3;
            if (line.get(i_start..i_end) == Some("--")) {
                // inputData.connections[ii].push((i-1, ii+1));
                // inputData.connections[ii+1].push((i-1, ii));
                inputData.connections[ii].push(Connexion::new(i-1, ii+1));
                inputData.connections[ii+1].push(Connexion::new(i-1, ii));
            }
        }
    }

    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    DBG_P(input_line.trim_right());

    inputData.output_labels = input_line.trim_right().split_whitespace().map(|s| s.to_string()).collect();
    
    inputData
}

#[derive(Debug)]
struct Connexion {
    // depth: i32,
    // line: i32
    depth: usize,
    line: usize
}

impl Connexion {

    // fn new(depth: i32, line: i32) -> Connexion {
    fn new(depth: usize, line: usize) -> Connexion {
        Connexion {
            depth: depth,
            line: line
        }
    }

    // fn to_tuple(&self) -> (i32, i32) {
    fn to_tuple(&self) -> (usize, usize) {
        (self.depth, self.line)
    }

}


// #[derive(Debug)]
struct InputData {
    w: i32,
    h: i32,
    input_labels: Vec<String>,
    // connections: Vec<Vec<(usize, usize)>>,
    connections: Vec<Vec<Connexion>>,
    output_labels: Vec<String>
}

impl InputData {

    fn new() -> InputData {
        InputData {
            w: 0,
            h: 0,
            input_labels: Vec::new(),
            connections: Vec::new(),
            output_labels: Vec::new()
        }
    }

}

impl fmt::Debug for InputData {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "## InputData ##\nw: {}\nh: {}\ninput_labels: {:?}\nconnections: {:?}\noutput_labels: {:?}",
               self.w,
               self.h,
               self.input_labels,
               self.connections,
               self.output_labels)
    }
}

fn main() {
    let inputData = ParseInput();
    let input_labels = &inputData.input_labels;
    let output_labels = &inputData.output_labels;

    let verbose = true;
    if verbose {
        eprintln!("inputData\n{:?}", inputData);
    }

    let answer = ComputeAnswer(inputData.input_labels.len(), inputData.connections);

    for (input_line_idx, output_line_idx) in answer.iter().enumerate() {
        println!("{}{}", input_labels[input_line_idx], output_labels[*output_line_idx]);
    }

}
