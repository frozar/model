use std::cmp::Ordering;
use std::io;
use std::str::FromStr;

macro_rules! parse_input {
    ($x:expr, $t:ident) => {
        $x.trim().parse::<$t>().unwrap()
    };
}

#[derive(Debug, Clone)]
enum Sign {
    Rock,
    Paper,
    Scissors,
    Lizard,
    Spock,
}

// impl Sign {
//     fn new(sign: String) -> Self {
//         match sign.as_str() {
//             "R" => Sign::Rock,
//             "P" => Sign::Paper,
//             "C" => Sign::Scissors,
//             "L" => Sign::Lizard,
//             "S" => Sign::Spock,
//             _ => panic!("Cannot create Sign from string \"{}\"", sign),
//         }
//     }
// }

#[derive(Debug)]
pub enum Never {}

impl FromStr for Sign {
    type Err = Never;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        use Sign::*;
        Ok(match s {
            "L" => Lizard,
            "P" => Paper,
            "R" => Rock,
            "C" => Scissors,
            "S" => Spock,
            _ => unreachable!(),
        })
    }
}

impl PartialEq for Sign {
    fn eq(&self, other: &Self) -> bool {
        self == other
    }
}

impl Eq for Sign {}

impl PartialOrd for Sign {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        match self {
            Sign::Rock => match other {
                Sign::Rock => Some(Ordering::Equal),
                Sign::Paper => Some(Ordering::Less),
                Sign::Scissors => Some(Ordering::Greater),
                Sign::Lizard => Some(Ordering::Greater),
                Sign::Spock => Some(Ordering::Less),
            },
            Sign::Paper => match other {
                Sign::Rock => Some(other.partial_cmp(self).unwrap().reverse()),
                Sign::Paper => Some(Ordering::Equal),
                Sign::Scissors => Some(Ordering::Less),
                Sign::Lizard => Some(Ordering::Less),
                Sign::Spock => Some(Ordering::Greater),
            },
            Sign::Scissors => match other {
                Sign::Rock => Some(other.partial_cmp(self).unwrap().reverse()),
                Sign::Paper => Some(other.partial_cmp(self).unwrap().reverse()),
                Sign::Scissors => Some(Ordering::Equal),
                Sign::Lizard => Some(Ordering::Greater),
                Sign::Spock => Some(Ordering::Less),
            },
            Sign::Lizard => match other {
                Sign::Rock => Some(other.partial_cmp(self).unwrap().reverse()),
                Sign::Paper => Some(other.partial_cmp(self).unwrap().reverse()),
                Sign::Scissors => Some(other.partial_cmp(self).unwrap().reverse()),
                Sign::Lizard => Some(Ordering::Equal),
                Sign::Spock => Some(Ordering::Greater),
            },
            Sign::Spock => match other {
                Sign::Rock => Some(other.partial_cmp(self).unwrap().reverse()),
                Sign::Paper => Some(other.partial_cmp(self).unwrap().reverse()),
                Sign::Scissors => Some(other.partial_cmp(self).unwrap().reverse()),
                Sign::Lizard => Some(other.partial_cmp(self).unwrap().reverse()),
                Sign::Spock => Some(Ordering::Equal),
            },
        }
    }
}

#[derive(Debug, Clone)]
struct Play {
    player_id: i32,
    sign: Sign,
    opponents: Vec<i32>,
}

impl PartialEq for Play {
    fn eq(&self, other: &Self) -> bool {
        self.player_id == other.player_id
            && self.sign == other.sign
            && self.opponents == other.opponents
    }
}

impl Eq for Play {}

impl PartialOrd for Play {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        if (self.sign < other.sign) {
            Some(Ordering::Less)
        } else if (self.sign > other.sign) {
            Some(Ordering::Greater)
        } else {
            if (self.player_id < other.player_id) {
                Some(Ordering::Greater)
            } else if (self.player_id > other.player_id) {
                Some(Ordering::Less)
            } else {
                Some(Ordering::Equal)
            }
        }
    }
}

fn parse_input() -> Vec<Play> {
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let n = parse_input!(input_line, i32);
    let mut plays: Vec<Play> = vec![];
    for i in 0..n as usize {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        let inputs = input_line.split(" ").collect::<Vec<_>>();
        let numplayer = parse_input!(inputs[0], i32);
        let signplayer = inputs[1].trim().to_string();
        plays.push(Play {
            player_id: numplayer,
            // sign: Sign::new(signplayer),
            sign: Sign::from_str(&signplayer).unwrap(),
            opponents: vec![],
        });
    }
    return plays;
}

fn tour(plays: &Vec<Play>) -> Vec<Play> {
    let mut new_plays: Vec<Play> = vec![];
    for i in (0..plays.len()).step_by(2) {
        if (plays[i] < plays[i + 1]) {
            let mut winner = plays[i + 1].clone();
            winner.opponents.push(plays[i].player_id);
            new_plays.push(winner);
        } else {
            let mut winner = plays[i].clone();
            winner.opponents.push(plays[i + 1].player_id);
            new_plays.push(winner);
        }
    }
    return new_plays;
}

fn main() {
    let plays: Vec<Play> = parse_input();
    // eprintln!("{:?}", plays);

    let mut plays_iterations: Vec<Play> = tour(&plays);
    // eprintln!("{:?}", plays_iterations);

    while (1 < plays_iterations.len()) {
        plays_iterations = tour(&plays_iterations);
        // eprintln!("{:?}", plays_iterations);
    }

    println!("{}", plays_iterations[0].player_id);
    println!(
        "{}",
        plays_iterations[0]
            .opponents
            .iter()
            .map(|n| n.to_string())
            .collect::<Vec<String>>()
            .join(" ")
    );
}
