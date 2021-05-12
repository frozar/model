// use itertools::Itertools;
use std::cmp::Ordering;
use std::collections::HashMap;
use std::io;

// use std::cmp;

// debug function

// fn print_board(board: &Vec<Cell>) {
//     for cell in board.iter() {
//         eprintln!("{:?}", cell);
//     }
// }

// fn print_idx_to_coord(coord: &HashMap<i32, (i32, i32, i32)>) {
//     // for (idx, tuple) in coord.iter() {
//     //     eprintln!("{} {:?}", idx, tuple);
//     // }
//     let mut keys = vec![];
//     for idx in coord.keys() {
//         keys.push(*idx);
//         // eprintln!("{} {:?}", *idx, coord.get(idx).unwrap());
//     }
//     keys.sort();
//     for idx in keys.iter() {
//         eprintln!("{} {:?}", *idx, coord.get(idx).unwrap());
//     }
// }

// fn compute_coord_to_idx(coord: &HashMap<i32, (i32, i32, i32)>) -> HashMap<(i32, i32, i32), i32> {
//     let mut coord_to_idx: HashMap<(i32, i32, i32), i32> = HashMap::new();
//     for (idx, tuple) in coord.iter() {
//         coord_to_idx.insert(*tuple, *idx);
//     }
//     coord_to_idx
// }

// fn print_coord_to_idx(coord_to_idx: &HashMap<(i32, i32, i32), i32>) {
//     for (tuple, idx) in coord_to_idx.iter() {
//         eprintln!("{:?} {}", tuple, idx);
//     }
// }
// <----- debug function

// Write an action using println!("message...");
// To debug: eprintln!("Debug message...");

macro_rules! parse_input {
    ($x:expr, $t:ident) => {
        $x.trim().parse::<$t>().unwrap()
    };
}

// macro_rules! min {
//     ($x: expr) => ($x);
//     ($x: expr, $($z: expr),+) => (::std::cmp::min($x, min!($($z),*)));
// }

macro_rules! max {
    ($x: expr) => ($x);
    ($x: expr, $($z: expr),+) => (::std::cmp::max($x, max!($($z),*)));
}

#[derive(Debug)]
struct Cell {
    index: i32,
    richness: i32,
    neigh: Vec<i32>,
}

#[derive(Debug)]
struct Tree {
    cell_index: i32,
    size: i32,
    is_mine: i32,
    is_dormant: i32,
}

#[derive(Debug)]
struct Game {
    day: i32,
    nutrients: i32,
    sun: i32,
    score: i32,
    opp_sun: i32,
    opp_score: i32,
    opp_is_waiting: i32,
    trees: Vec<Tree>,
}

#[derive(Debug, Clone, Copy)]
struct Coord(i32, i32, i32);

const SEED_ID: i32 = 0;
const GROW_ID: i32 = 1;
const COMPLETE_ID: i32 = 2;

const BEGIN_GAME_DAY: i32 = 5;
const MIDDLE_GAME_DAY: i32 = 11;
const END_GAME_DAY: i32 = 16;

const DIRECTION: &'static [Coord] = &[
    Coord(1, -1, 0),
    Coord(1, 0, -1),
    Coord(0, 1, -1),
    Coord(-1, 1, 0),
    Coord(-1, 0, 1),
    Coord(0, -1, 1),
];

impl Coord {
    fn add(&self, other: &Coord) -> Coord {
        // self.width * self.height
        Coord(self.0 + other.0, self.1 + other.1, self.2 + other.2)
    }

    fn sub(&self, other: &Coord) -> Coord {
        // self.width * self.height
        Coord(self.0 - other.0, self.1 - other.1, self.2 - other.2)
    }

    fn cross_product(&self, other: &Coord) -> Coord {
        Coord(
            self.1 * other.2 - other.1 * self.2,
            self.2 * other.0 - other.2 * self.0,
            self.0 * other.1 - other.0 * self.1,
        )
    }

    fn is_null(&self) -> bool {
        self.0.abs() + self.1.abs() + self.2.abs() == 0
    }

    fn is_colinear(&self, other: &Coord) -> bool {
        self.cross_product(other).is_null()
    }

    fn are_aligned(&self, other: &Coord) -> bool {
        let vec01 = other.sub(self);
        DIRECTION
            .iter()
            .map(|direction| direction.is_colinear(&vec01))
            .reduce(|b0, b1| b0 || b1)
            .unwrap()
        // vec01.are_colinear()
        // true
    }

    fn dist(&self, other: &Coord) -> i32 {
        // self.width * self.height
        max!(
            (self.0 - other.0).abs(),
            (self.1 - other.1).abs(),
            (self.2 - other.2).abs()
        )
    }
}

fn parse_input_initialisation() -> Vec<Cell> {
    let mut cells = vec![];

    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    // 37 cells over the board
    let number_of_cells = parse_input!(input_line, i32);
    for _ in 0..number_of_cells as usize {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        let inputs = input_line.split(" ").collect::<Vec<_>>();
        // 0 is the center cell, the next cells spiral outwards
        let index = parse_input!(inputs[0], i32);
        // 0 if the cell is unusable, 1-3 for usable cells
        let richness = parse_input!(inputs[1], i32);
        // the index of the neighbouring cell for each direction
        let neigh_0 = parse_input!(inputs[2], i32);
        let neigh_1 = parse_input!(inputs[3], i32);
        let neigh_2 = parse_input!(inputs[4], i32);
        let neigh_3 = parse_input!(inputs[5], i32);
        let neigh_4 = parse_input!(inputs[6], i32);
        let neigh_5 = parse_input!(inputs[7], i32);
        let neigh = vec![neigh_0, neigh_1, neigh_2, neigh_3, neigh_4, neigh_5];
        let cell = Cell {
            index,
            richness,
            neigh,
        };
        cells.push(cell);
    }

    cells
}

// Documentation link:
// https://www.redblobgames.com/grids/hexagons/
fn compute_idx_to_coord(board: &Vec<Cell>) -> HashMap<i32, Coord> {
    let mut idx_to_coord: HashMap<i32, Coord> = HashMap::new();
    for (i, cell) in board.iter().enumerate() {
        if i == 0 {
            idx_to_coord.insert(0, Coord(0, 0, 0));
            continue;
        }

        let mut known_neigh: (usize, Coord) = (0, Coord(0, 0, 0));
        for (direction, neigh) in cell.neigh.iter().enumerate() {
            match idx_to_coord.get(&neigh) {
                Some(&t) => {
                    known_neigh = (direction, t);
                    break;
                }
                _ => {}
            }
        }

        let (come_from_direction, coord) = known_neigh;

        let new_coord = coord.sub(&DIRECTION[come_from_direction]);
        idx_to_coord.insert(cell.index, new_coord);
    }
    idx_to_coord
}

fn get_cell_under_dist(
    cell_index: i32,
    distance_thresold: i32,
    idx_to_coord: &HashMap<i32, Coord>,
) -> Vec<i32> {
    let cell_tuple = idx_to_coord.get(&cell_index).unwrap();
    let mut neigh_idx: Vec<i32> = vec![];
    for (idx, tuple) in idx_to_coord.iter() {
        if *idx == cell_index {
            continue;
        }

        if cell_tuple.dist(tuple) <= distance_thresold {
            neigh_idx.push(*idx);
        }
    }

    neigh_idx
}

// fn get_cell_at_exact_dist(
//     cell_index: i32,
//     distance: i32,
//     idx_to_coord: &HashMap<i32, (i32, i32, i32)>,
// ) -> Vec<i32> {
//     let cell_tuple = idx_to_coord.get(&cell_index).unwrap();
//     let mut neigh_idx: Vec<i32> = vec![];
//     for (idx, tuple) in idx_to_coord.iter() {
//         if *idx == cell_index {
//             continue;
//         }

//         if dist_coord(*cell_tuple, *tuple) == distance {
//             neigh_idx.push(*idx);
//         }
//     }

//     neigh_idx
// }

// fn get_free_cell_at_distance(
//     board: &Vec<Cell>,
//     game: &Game,
//     tree: &Tree,
//     distance: i32,
//     idx_to_coord: &HashMap<i32, (i32, i32, i32)>,
// ) -> Vec<i32> {
//     // eprintln!("grow {:?}", *tree);
//     let mut neigh = get_cell_at_exact_dist(tree.cell_index, distance, idx_to_coord)
//         .into_iter()
//         .filter(|cell_index| board[*cell_index as usize].richness != 0)
//         .filter(|cell_index| {
//             let mut not_found = true;
//             for tree in game.trees.iter() {
//                 if tree.cell_index == *cell_index {
//                     not_found = false;
//                     break;
//                 }
//             }
//             not_found
//         })
//         .collect::<Vec<i32>>();
//     // eprintln!("neigh {:?}", neigh);
//     neigh.sort_by(|cell_index0, cell_index1| {
//         // board[*cell_index0 as usize].richness.cmp(board[*cell_index1 as usize].richness)
//         let r0 = board[*cell_index0 as usize].richness;
//         let r1 = board[*cell_index1 as usize].richness;
//         r0.cmp(&r1)
//     });
//     neigh.reverse();
//     neigh
// }

fn parse_input_loop() -> Game {
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    // the game lasts 24 days: 0-23
    let day = parse_input!(input_line, i32);
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    // the base score you gain from the next COMPLETE action
    let nutrients = parse_input!(input_line, i32);
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let inputs = input_line.split(" ").collect::<Vec<_>>();
    // your sun points
    let sun = parse_input!(inputs[0], i32);
    // your current score
    let score = parse_input!(inputs[1], i32);
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let inputs = input_line.split(" ").collect::<Vec<_>>();
    // opponent's sun points
    let opp_sun = parse_input!(inputs[0], i32);
    // opponent's score
    let opp_score = parse_input!(inputs[1], i32);
    // whether your opponent is asleep until the next day
    let opp_is_waiting = parse_input!(inputs[2], i32);
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    // the current amount of trees
    let number_of_trees = parse_input!(input_line, i32);
    let mut trees = vec![];
    for _ in 0..number_of_trees as usize {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        let inputs = input_line.split(" ").collect::<Vec<_>>();
        // location of this tree
        let cell_index = parse_input!(inputs[0], i32);
        // size of this tree: 0-3
        let size = parse_input!(inputs[1], i32);
        // 1 if this is your tree
        let is_mine = parse_input!(inputs[2], i32);
        // 1 if this tree is dormant
        let is_dormant = parse_input!(inputs[3], i32);
        let tree = Tree {
            cell_index,
            size,
            is_mine,
            is_dormant,
        };
        trees.push(tree);
    }
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    // all legal actions
    let number_of_possible_actions = parse_input!(input_line, i32);

    for _ in 0..number_of_possible_actions as usize {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        input_line.trim_matches('\n').to_string();
    }

    Game {
        day,
        nutrients,
        sun,
        score,
        opp_sun,
        opp_score,
        opp_is_waiting,
        trees,
    }
}

fn count_tree(trees: &Vec<Tree>, size: i32) -> i32 {
    let mut nb_tree = 0;
    for tree in trees.iter() {
        if tree.size == size && tree.is_mine == 1 {
            nb_tree += 1;
        }
    }
    nb_tree
}

fn grow_cost(tree: &Tree, game: &Game) -> i32 {
    if tree.size == 3 || tree.is_mine != 1 {
        -1
    } else {
        let count_tree = count_tree(&game.trees, tree.size + 1);
        let grow_cost = if tree.size == 0 {
            1
        } else if tree.size == 1 {
            3
        } else {
            7
        };
        count_tree + grow_cost
    }
}

fn compute_possible_action(
    board: &Vec<Cell>,
    game: &Game,
    // idx_to_coord: &HashMap<i32, (i32, i32, i32)>,
) -> Vec<Vec<i32>> {
    let trees = &game.trees;
    let nb_sun = game.sun;
    let mut possible_actions = vec![];
    // eprintln!("nb_sun {}", nb_sun);

    for tree in trees.iter() {
        if tree.is_mine != 1 || tree.is_dormant == 1 {
            continue;
        }

        let nb_tree_size_0 = count_tree(&game.trees, 0);
        if nb_tree_size_0 <= nb_sun {
            possible_actions.push(vec![SEED_ID, tree.cell_index]);
        }

        if tree.size == 3 {
            if 4 <= nb_sun {
                possible_actions.push(vec![COMPLETE_ID, tree.cell_index]);
            }
        } else {
            let grow_cost = grow_cost(&tree, &game);
            // eprintln!("tree {:?} grow_cost {1}", tree, grow_cost);
            if 0 <= grow_cost && grow_cost <= nb_sun {
                possible_actions.push(vec![GROW_ID, tree.cell_index]);
            }
        }
    }

    // TODO: sort by tree size, not cell_index
    possible_actions.sort_by(|a0, a1| -> Ordering {
        let mut res = Ordering::Equal;
        if a0[0] < a1[0] {
            res = Ordering::Less;
        }
        if a0[0] > a1[0] {
            res = Ordering::Greater;
        }
        if res == Ordering::Equal {
            let action_id = a0[0];
            let cell_index0 = a0[1] as usize;
            let cell_index1 = a1[1] as usize;
            let t0 = get_tree_at_cell_index(game, cell_index0 as i32);
            let t1 = get_tree_at_cell_index(game, cell_index1 as i32);
            let r0 = board[a0[1] as usize].richness;
            let r1 = board[a1[1] as usize].richness;
            // si GROW: favorise les cell les plus riches
            if action_id == GROW_ID || action_id == COMPLETE_ID {
                if r0 < r1 {
                    res = Ordering::Less;
                }
                if r0 > r1 {
                    res = Ordering::Greater;
                }
            } else {
                if t0.size < t1.size {
                    res = Ordering::Less;
                }
                if t0.size > t1.size {
                    res = Ordering::Greater;
                }
            }
            if res == Ordering::Equal {
                let r0 = board[a0[1] as usize].richness;
                let r1 = board[a1[1] as usize].richness;
                if r0 < r1 {
                    res = Ordering::Less;
                }
                if r0 > r1 {
                    res = Ordering::Greater;
                }
                res
            } else {
                res
            }
        } else {
            res
        }
    });
    possible_actions.reverse();

    possible_actions
}

fn get_tree_at_cell_index(game: &Game, cell_index: i32) -> &Tree {
    game.trees
        .iter()
        .filter(|tree| tree.cell_index == cell_index)
        .collect::<Vec<&Tree>>()[0]
}

// fn get_my_trees(game: &Game) -> Vec<&Tree> {
//     game.trees
//         .iter()
//         .filter(|tree| tree.is_mine == 1)
//         .collect::<Vec<&Tree>>()
// }

fn get_free_cell_under_distance(
    board: &Vec<Cell>,
    game: &Game,
    tree: &Tree,
    distance: i32,
    idx_to_coord: &HashMap<i32, Coord>,
) -> Vec<i32> {
    // eprintln!("grow {:?}", *tree);
    let mut neigh = get_cell_under_dist(tree.cell_index, distance, idx_to_coord)
        .into_iter()
        .filter(|cell_index| board[*cell_index as usize].richness != 0)
        .filter(|cell_index| {
            let mut not_found = true;
            for tree in game.trees.iter() {
                if tree.cell_index == *cell_index {
                    not_found = false;
                    break;
                }
            }
            not_found
        })
        .collect::<Vec<i32>>();
    // eprintln!("neigh {:?}", neigh);
    neigh.sort_by(|cell_index0, cell_index1| {
        // board[*cell_index0 as usize].richness.cmp(board[*cell_index1 as usize].richness)
        let r0 = board[*cell_index0 as usize].richness;
        let r1 = board[*cell_index1 as usize].richness;
        r0.cmp(&r1)
    });
    neigh.reverse();
    neigh
}

fn try_action(
    board: &Vec<Cell>,
    game: &Game,
    idx_to_coord: &HashMap<i32, Coord>,
    possible_actions: &Vec<Vec<i32>>,
) -> String {
    let mut res = String::from("");
    // for action in possible_actions.iter() {
    //     eprintln!("action {:?}", action);
    // }
    for action in possible_actions.iter() {
        // eprintln!("action {:?}", action);
        let action_id = action[0];
        match action_id {
            SEED_ID => {
                if game.day <= END_GAME_DAY {
                    let cell_index = action[1];
                    // eprintln!("cell_index {:?}", cell_index);
                    let tree = get_tree_at_cell_index(game, cell_index);
                    // let my_trees = get_my_trees(game);
                    let mut neigh =
                        get_free_cell_under_distance(board, game, tree, tree.size, idx_to_coord)
                            .into_iter()
                            .filter(|free_cell_index| {
                                if game.day <= MIDDLE_GAME_DAY {
                                    // eprintln!("free_cell_index {:?}", free_cell_index);
                                    let coord_target = idx_to_coord.get(free_cell_index).unwrap();
                                    // Check if the current free cell is not aligned with any trees
                                    // let one_tree = vec![tree];
                                    let trees_to_check = if game.day <= BEGIN_GAME_DAY {
                                        game.trees.iter().collect::<Vec<&Tree>>()
                                    } else {
                                        vec![tree]
                                    };
                                    let has_annoying_tree = trees_to_check
                                        .iter()
                                        .map(|tree| {
                                            let coord_tree =
                                                idx_to_coord.get(&tree.cell_index).unwrap();
                                            // eprintln!("tree.cell_index {:?}", tree.cell_index);
                                            if tree.size < coord_tree.dist(coord_target) {
                                                false
                                            } else {
                                                coord_tree.are_aligned(coord_target)
                                            }
                                        })
                                        .collect::<Vec<bool>>();
                                    // eprintln!("has_any_tree_aligned {:?}", has_annoying_tree);
                                    !has_annoying_tree
                                        .into_iter()
                                        .reduce(|b0, b1| b0 || b1)
                                        .unwrap()
                                } else {
                                    true
                                }
                            })
                            .collect::<Vec<i32>>();
                    neigh.sort_by(|cell_index0, cell_index1| {
                        let cell0 = &board[*cell_index0 as usize];
                        let cell1 = &board[*cell_index1 as usize];
                        cell0.richness.cmp(&cell1.richness)
                    });
                    neigh.reverse();
                    // TODO: sort by ceil richness

                    // eprintln!("");
                    if neigh.len() != 0 {
                        res = String::from(format!("SEED {} {}", cell_index, neigh[0]));
                    }
                }
            }
            GROW_ID => {
                let cell_index = action[1];
                res = String::from(format!("GROW {}", cell_index));
            }
            COMPLETE_ID => {
                if END_GAME_DAY < game.day {
                    let cell_index = action[1];
                    res = String::from(format!("COMPLETE {}", cell_index));
                }
            }
            _ => {
                res = String::from("WAIT");
            }
        };
        if res != String::from("") {
            break;
        }
    }

    if res == String::from("") {
        res = String::from("WAIT");
    }
    res
}

fn play(board: &Vec<Cell>, game: &Game, idx_to_coord: &HashMap<i32, Coord>) {
    let possible_actions: Vec<Vec<i32>> = compute_possible_action(board, game); //, idx_to_coord);

    if possible_actions.len() == 0 {
        println!("WAIT");
    } else {
        let action_string = try_action(board, game, idx_to_coord, &possible_actions);
        println!("{}", action_string);
    }
}

fn main() {
    let board: Vec<Cell> = parse_input_initialisation();

    let idx_to_coord: HashMap<i32, Coord> = compute_idx_to_coord(&board);

    // game loop
    loop {
        let game = parse_input_loop();
        play(&board, &game, &idx_to_coord);
    }
}
