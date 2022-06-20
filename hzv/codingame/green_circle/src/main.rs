use std::borrow::BorrowMut;
use std::io;

macro_rules! parse_input {
    ($x:expr, $t:ident) => {
        $x.trim().parse::<$t>().unwrap()
    };
}

macro_rules! e {
    () => {};
    ($($x:tt),* $(,)?) => {
        eprintln!($($x),*)
    };
}

// ##################################################### DATA STRUCTURE

const NB_DIFFERENT_NEED: usize = 8;
const NB_DIFFERENT_CARD: usize = 10;

#[derive(Debug, Eq, Hash, PartialEq)]
enum Need {
    training,
    coding,
    daily_routine,
    task_prioritization,
    architecture_study,
    continuous_delivery,
    code_review,
    refactoring,
}

#[derive(Debug, Eq, Hash, PartialEq)]
enum Card {
    training,
    coding,
    daily_routine,
    task_prioritization,
    architecture_study,
    continuous_delivery,
    code_review,
    refactoring,
    bonus,
    technical_debt,
}

#[derive(Debug)]
struct Application {
    id: i32,
    needs: [i32; 8],
}

impl core::cmp::PartialEq for Application {
    fn eq(&self, other: &Self) -> bool {
        self.id == other.id && self.needs == other.needs
    }
}

impl Need {
    pub fn position(&self) -> isize {
        match self {
            Need::training => 0,
            Need::coding => 1,
            Need::daily_routine => 2,
            Need::task_prioritization => 3,
            Need::architecture_study => 4,
            Need::continuous_delivery => 5,
            Need::code_review => 6,
            Need::refactoring => 7,
        }
    }

    pub fn from_position(i: usize) -> Need {
        match i {
            0 => Need::training,
            1 => Need::coding,
            2 => Need::daily_routine,
            3 => Need::task_prioritization,
            4 => Need::architecture_study,
            5 => Need::continuous_delivery,
            6 => Need::code_review,
            7 => Need::refactoring,
            _ => panic!("No need for this position"),
        }
    }
}

impl Card {
    pub fn position(&self) -> isize {
        match self {
            Card::training => 0,
            Card::coding => 1,
            Card::daily_routine => 2,
            Card::task_prioritization => 3,
            Card::architecture_study => 4,
            Card::continuous_delivery => 5,
            Card::code_review => 6,
            Card::refactoring => 7,
            Card::bonus => 8,
            Card::technical_debt => 9,
        }
    }

    pub fn from_position(i: usize) -> Card {
        match i {
            0 => Card::training,
            1 => Card::coding,
            2 => Card::daily_routine,
            3 => Card::task_prioritization,
            4 => Card::architecture_study,
            5 => Card::continuous_delivery,
            6 => Card::code_review,
            7 => Card::refactoring,
            8 => Card::bonus,
            9 => Card::technical_debt,
            _ => panic!("No need for this position"),
        }
    }
}

#[derive(Debug, Default)]
struct Player {
    location: i32,
    score: i32,
    daily_routine: i32,
    architecture_study: i32,
}

use std::collections::HashMap;

#[derive(Debug, Default)]
struct Input {
    game_phase: String,
    applications: Vec<Application>,
    me: Player,
    opponent: Player,
    hand: HashMap<Card, i32>,
    draw: HashMap<Card, i32>,
    discard: HashMap<Card, i32>,
    opponent_cards: HashMap<Card, i32>,
    automated: HashMap<Card, i32>,
    opponent_automated: HashMap<Card, i32>,
    possible_moves: Vec<String>,
}

// ##################################################### UTILITIES

fn most_popular_need(applications: &Vec<Application>) -> Need {
    let mut every_needs: [i32; NB_DIFFERENT_NEED] = [0; NB_DIFFERENT_NEED];

    for app in applications.iter() {
        for (i, need) in app.needs.iter().enumerate() {
            every_needs[i] += need;
        }
    }

    let mut position_max_need = 0;
    let mut max_need = every_needs[0];
    for (i, every_need) in every_needs.iter().enumerate() {
        if max_need < *every_need {
            max_need = *every_need;
            position_max_need = i;
        }
    }
    Need::from_position(position_max_need)
}

fn closest_app(input: &Input) -> &Application {
    let mut competences = HashMap::<Card, i32>::new();

    // 0 - Sum every competences from "hand" and "draw"
    for i in 0..NB_DIFFERENT_CARD {
        competences.insert(
            Card::from_position(i),
            *input.hand.get(&Card::from_position(i)).unwrap()
                + *input.draw.get(&Card::from_position(i)).unwrap(),
        );
    }

    // 1 - Find closest application of the competence I have
    struct ApplicationScore<'a> {
        app: &'a Application,
        score: i32,
    }

    impl core::cmp::PartialEq for ApplicationScore<'_> {
        fn eq(&self, other: &Self) -> bool {
            *self.app == *other.app && self.score == other.score
        }
    }

    impl core::cmp::PartialOrd for ApplicationScore<'_> {
        fn partial_cmp(&self, other: &Self) -> Option<core::cmp::Ordering> {
            if self.score < other.score {
                Some(std::cmp::Ordering::Less)
            } else if self.score > other.score {
                Some(std::cmp::Ordering::Greater)
            } else {
                Some(std::cmp::Ordering::Equal)
            }
        }
    }

    let mut array = vec![];

    for app in input.applications.iter() {
        let mut score = 0;
        for i in 0..NB_DIFFERENT_CARD {
            if i < NB_DIFFERENT_NEED {
                score += competences.get(&Card::from_position(i)).unwrap() * 2 - app.needs[i];
            } else {
                match Card::from_position(i) {
                    Card::bonus => score += 1,
                    Card::technical_debt => (),
                    _ => panic!("Doesn't deal with this card"),
                }
            }
        }
        array.push(ApplicationScore { app, score });
    }

    array.sort_unstable_by(|a, b| a.partial_cmp(b).unwrap());

    // &input.applications[0]
    array[0].app
}

fn parse_inputs() -> Input {
    let mut input: Input = Input::default();

    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();

    // can be MOVE, GIVE_CARD, THROW_CARD, PLAY_CARD or RELEASE
    input.game_phase = input_line.trim().to_string();

    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();

    let applications_count = parse_input!(input_line, i32);

    for _ in 0..applications_count as usize {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();

        let inputs = input_line.split(" ").collect::<Vec<_>>();
        let _ = inputs[0].trim().to_string();
        let id = parse_input!(inputs[1], i32);
        let training_needed = parse_input!(inputs[2], i32);
        let coding_needed = parse_input!(inputs[3], i32);
        let daily_routine_needed = parse_input!(inputs[4], i32);
        let task_prioritization_needed = parse_input!(inputs[5], i32);
        let architecture_study_needed = parse_input!(inputs[6], i32);
        let continuous_delivery_needed = parse_input!(inputs[7], i32);
        let code_review_needed = parse_input!(inputs[8], i32);
        let refactoring_needed = parse_input!(inputs[9], i32);

        input.applications.push(Application {
            id,
            needs: [
                training_needed,
                coding_needed,
                daily_routine_needed,
                task_prioritization_needed,
                architecture_study_needed,
                continuous_delivery_needed,
                code_review_needed,
                refactoring_needed,
            ],
        });
    }

    for i in 0..2 as usize {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();

        let inputs = input_line.split(" ").collect::<Vec<_>>();
        let player_location = parse_input!(inputs[0], i32);
        let player_score = parse_input!(inputs[1], i32);
        let player_permanent_daily_routine_cards = parse_input!(inputs[2], i32);
        let player_permanent_architecture_study_cards = parse_input!(inputs[3], i32);

        if i == 0 {
            input.me = Player {
                location: player_location,
                score: player_score,
                daily_routine: player_permanent_daily_routine_cards,
                architecture_study: player_permanent_architecture_study_cards,
            };
        } else {
            input.opponent = Player {
                location: player_location,
                score: player_score,
                daily_routine: player_permanent_daily_routine_cards,
                architecture_study: player_permanent_architecture_study_cards,
            };
        }
    }

    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();

    let card_locations_count = parse_input!(input_line, i32);
    for _ in 0..card_locations_count as usize {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();

        let inputs = input_line.split(" ").collect::<Vec<_>>();
        let cards_location = inputs[0].trim().to_string();

        let input_cards_location: &mut HashMap<Card, i32> = match cards_location.as_str() {
            "HAND" => input.hand.borrow_mut(),
            "DRAW" => input.draw.borrow_mut(),
            "DISCARD" => input.discard.borrow_mut(),
            "OPPONENT_CARDS" => input.opponent_cards.borrow_mut(),
            "AUTOMATED" => input.automated.borrow_mut(),
            "OPPONENT_AUTOMATED" => input.opponent_automated.borrow_mut(),
            &_ => panic!("Don't know this cards location"),
        };

        for i in 0..10 {
            let card_count = parse_input!(inputs[i + 1], i32);
            input_cards_location.insert(Card::from_position(i), card_count);
        }
    }

    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();

    let possible_moves_count = parse_input!(input_line, i32);
    for _ in 0..possible_moves_count as usize {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();

        let possible_move = input_line.trim_matches('\n').to_string();

        input.possible_moves.push(possible_move);
    }

    input
}

// ##################################################### MAIN

fn main() {
    let mut i = 0;
    let mut j = 0;

    // game loop
    loop {
        let input = parse_inputs();

        dbg!(&input.game_phase);
        for app in input.applications.iter() {
            dbg!(app);
        }

        for possible_move in input.possible_moves.iter() {
            dbg!(possible_move);
        }

        // In the first league: RANDOM | MOVE <zoneId> | RELEASE <applicationId> | WAIT; In later leagues: | GIVE <cardType> | THROW <cardType> | TRAINING | CODING | DAILY_ROUTINE | TASK_PRIORITIZATION <cardTypeToThrow> <cardTypeToTake> | ARCHITECTURE_STUDY | CONTINUOUS_DELIVERY <cardTypeToAutomate> | CODE_REVIEW | REFACTORING;
        // println!("RANDOM");
        dbg!(i);

        let most_popular_need = most_popular_need(&input.applications);
        dbg!(most_popular_need.position());

        if input.game_phase == "MOVE" {
            println!("MOVE {}", most_popular_need.position());
        } else {
            if j % 2 == 1 {
                println!("RANDOM");
            } else {
                println!("WAIT");
            }
            j += 1;
        }

        i = (i + 1) % 8;
    }
}
