use std::cmp;
use std::io;

macro_rules! parse_input {
    ($x:expr, $t:ident) => {
        $x.trim().parse::<$t>().unwrap()
    };
}

fn parse_input() -> (Vec<Vec<char>>, i32, i32) {
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let w = parse_input!(input_line, i32);
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let h = parse_input!(input_line, i32);
    eprintln!("w {}", w);
    eprintln!("h {}", h);
    let mut array_2d: Vec<Vec<char>> = vec![];
    for _ in 0..h {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        let line = input_line.trim_matches('\n').to_string();
        eprintln!("in line {}", line);
        let mut vec_line = vec![];
        for c in line.chars() {
            vec_line.push(c);
        }
        // eprintln!("vec_line {:?}", vec_line);
        // eprintln!("vec_line.len() {:?}", vec_line.len());
        array_2d.push(vec_line.clone());
    }

    return (array_2d, w, h);
}

#[derive(Debug, PartialEq)]
struct Coord {
    i: i32,
    j: i32,
}

fn compute_mine_positions(array_2d: &Vec<Vec<char>>) -> Vec<Coord> {
    let mut res = vec![];
    for (i, vec_line) in array_2d.iter().enumerate() {
        // println!("In position {} we have value {:?}", i, vec_line);
        for (j, c) in vec_line.iter().enumerate() {
            if (*c == 'x') {
                res.push(Coord {
                    i: (i as i32),
                    j: (j as i32),
                });
            }
        }
    }
    return res;
}

fn is_mine_position(coord: &Coord, mine_positions: &Vec<Coord>) -> bool {
    for coord_mine in mine_positions {
        if (*coord_mine == *coord) {
            return true;
        }
    }
    return false;
}

fn compute_solution(array_2d: &mut Vec<Vec<char>>, mine_positions: &Vec<Coord>, w: i32, h: i32) {
    for coord in mine_positions.iter() {
        // eprintln!("coord {:?}", coord);
        for i in cmp::max(((coord.i as i32) - 1), 0)..cmp::min((coord.i + 1 + 1), h) {
            // eprintln!("i {:?}", i);
            for j in cmp::max(((coord.j as i32) - 1), 0)..cmp::min((coord.j + 1 + 1), w) {
                // eprintln!("j {:?}", j);

                let i_u = i as usize;
                let j_u = j as usize;

                if (is_mine_position(&Coord { i: i, j: j }, &mine_positions)) {
                    array_2d[i_u][j_u] = '.';
                } else if (array_2d[i_u][j_u] == '.') {
                    array_2d[i_u][j_u] = '1';
                } else {
                    let nb_mine_incremented =
                        (array_2d[i_u][j_u].to_string().parse::<i32>().unwrap() + 1).to_string();
                    array_2d[i_u][j_u] = nb_mine_incremented.chars().next().unwrap();
                }
            }
        }
    }
}

fn main() {
    let (mut array_2d, w, h) = parse_input();
    let mine_positions = compute_mine_positions(&array_2d);
    compute_solution(&mut array_2d, &mine_positions, w, h);

    let mut answer: Vec<String> = vec![];

    for iter_line in array_2d.iter() {
        let str_line = iter_line
            .iter()
            .map(|n| n.to_string())
            .collect::<Vec<String>>()
            .join("");
        eprintln!("str_line {:?}", str_line);
        answer.push(str_line);
    }

    for str_line in answer.iter() {
        println!("{}", str_line);
    }
}
