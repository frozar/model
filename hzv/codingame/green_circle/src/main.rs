use std::borrow::BorrowMut;
use std::cmp::Ordering;
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

#[derive(Debug, Eq, Hash, PartialEq)]
enum Location {
    initial,
    training,
    coding,
    daily_routine,
    task_prioritization,
    architecture_study,
    continuous_delivery,
    code_review,
    refactoring,
    poste_administratif,
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
            _ => panic!("No card for this position"),
        }
    }
}

impl Location {
    pub fn position(&self) -> isize {
        match self {
            Location::initial => -1,
            Location::training => 0,
            Location::coding => 1,
            Location::daily_routine => 2,
            Location::task_prioritization => 3,
            Location::architecture_study => 4,
            Location::continuous_delivery => 5,
            Location::code_review => 6,
            Location::refactoring => 7,
            Location::poste_administratif => 8,
        }
    }

    pub fn from_position(i: i32) -> Location {
        match i {
            -1 => Location::initial,
            0 => Location::training,
            1 => Location::coding,
            2 => Location::daily_routine,
            3 => Location::task_prioritization,
            4 => Location::architecture_study,
            5 => Location::continuous_delivery,
            6 => Location::code_review,
            7 => Location::refactoring,
            8 => Location::poste_administratif,
            _ => panic!("No location for this position"),
        }
    }
}

trait Distance<Rhs: ?Sized = Self> {
    fn dist(&self, other: &Rhs) -> i32;
}

impl Distance for Location {
    fn dist(&self, other: &Location) -> i32 {
        if self.position() == -1 {
            1
        } else {
            other.position() as i32 - self.position() as i32
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

#[derive(Default)]
struct Cards(HashMap<Card, i32>);

impl std::fmt::Debug for Cards {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("Cards")
            .field("training", self.0.get(&Card::training).unwrap_or(&0))
            .field("coding", &self.0.get(&Card::coding).unwrap_or(&0))
            .field(
                "daily_routine",
                &self.0.get(&Card::daily_routine).unwrap_or(&0),
            )
            .field(
                "task_prioritization",
                &self.0.get(&Card::task_prioritization).unwrap_or(&0),
            )
            .field(
                "architecture_study",
                &self.0.get(&Card::architecture_study).unwrap_or(&0),
            )
            .field(
                "continuous_delivery",
                &self.0.get(&Card::continuous_delivery).unwrap_or(&0),
            )
            .field("code_review", &self.0.get(&Card::code_review).unwrap_or(&0))
            .field("refactoring", &self.0.get(&Card::refactoring).unwrap_or(&0))
            .field("bonus", &self.0.get(&Card::bonus).unwrap_or(&0))
            .field(
                "technical_debt",
                &self.0.get(&Card::technical_debt).unwrap_or(&0),
            )
            .finish()
    }
}

#[derive(Debug, Default)]
struct Input {
    game_phase: String,
    applications: Vec<Application>,
    me: Player,
    opponent: Player,
    hand: Cards,
    draw: Cards,
    discard: Cards,
    opponent_cards: Cards,
    automated: Cards,
    opponent_automated: Cards,
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

fn compute_competence(input: &Input) -> Cards {
    // let mut competences = HashMap::<Card, i32>::new();
    let mut competences = Cards::default();

    // 0 - Sum every competences from "hand" and "draw"
    for i in 0..NB_DIFFERENT_CARD {
        let competence_hand = match input.hand.0.get(&Card::from_position(i)) {
            Some(score) => *score,
            None => 0,
        };
        let competence_draw = match input.draw.0.get(&Card::from_position(i)) {
            Some(score) => *score,
            None => 0,
        };

        competences
            .0
            .insert(Card::from_position(i), competence_hand + competence_draw);
    }

    competences
}

// input: &Input
fn get_closest_app<'a>(input: &'a Input, competences: &'_ Cards) -> (&'a Application, i32) {
    // 0 - Sum every competences from "hand" and "draw"
    // let competences = compute_competence(&input);
    // dbg!(&competences);

    // 1 - Find closest application of the competence I have
    #[derive(Debug)]
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
        fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
            if self.score < other.score {
                Some(Ordering::Less)
            } else if self.score > other.score {
                Some(Ordering::Greater)
            } else {
                Some(Ordering::Equal)
            }
        }
    }

    let mut array_application_score = vec![];

    for app in input.applications.iter() {
        let mut score = 0;
        for i in 0..NB_DIFFERENT_CARD {
            if i < NB_DIFFERENT_NEED {
                if 0 < app.needs[i] {
                    score += competences.0.get(&Card::from_position(i)).unwrap() * 2 - app.needs[i];
                }
            } else {
                score += match Card::from_position(i) {
                    Card::bonus => competences.0.get(&Card::from_position(i)).unwrap(),
                    Card::technical_debt => &0,
                    _ => panic!("Doesn't deal with this card"),
                }
            }
            // dbg!(i, score);
        }
        // dbg!(app);
        // dbg!(score);
        array_application_score.push(ApplicationScore { app, score });
    }

    array_application_score.sort_unstable_by(|a, b| a.partial_cmp(b).unwrap());
    array_application_score.reverse();

    // for elt in array_application_score.iter() {
    //     dbg!(elt.app.id);
    //     dbg!(elt.app.needs);
    //     dbg!(elt.score);
    // }

    // dbg!(&array_application_score);

    // &input.applications[0]
    (
        array_application_score[0].app,
        array_application_score[0].score,
    )
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
            "HAND" => input.hand.0.borrow_mut(),
            "DRAW" => input.draw.0.borrow_mut(),
            "DISCARD" => input.discard.0.borrow_mut(),
            "OPPONENT_CARDS" => input.opponent_cards.0.borrow_mut(),
            "AUTOMATED" => input.automated.0.borrow_mut(),
            "OPPONENT_AUTOMATED" => input.opponent_automated.0.borrow_mut(),
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
    // game loop
    loop {
        let input = parse_inputs();

        dbg!(&input.game_phase);

        // let competences = compute_competence(&input);
        // let (closest_app, score_closest_app) = get_closest_app(&competences);
        let (closest_app, score_closest_app) = get_closest_app(&input, &input.hand);

        if input.game_phase == "MOVE" {
            // let competences = compute_competence(&input);
            // dbg!(&competences);

            dbg!(&input.hand);
            dbg!(&closest_app);
            dbg!(&score_closest_app);

            // let most_popular_need = most_popular_need(&input.applications);
            // dbg!(most_popular_need.position());

            dbg!(input.me.location);

            let sweet_locations = closest_app
                .needs
                .iter()
                .enumerate()
                .filter(|(idx, requierement)| {
                    let card = Card::from_position(*idx);
                    let competence_value = input.hand.0.get(&card).unwrap() * 2;

                    let need = **requierement - competence_value;

                    // **requierement != 0
                    0 < need
                })
                .map(|(idx, _)| Location::from_position(idx as i32))
                .collect::<Vec<Location>>();

            dbg!(&sweet_locations);
            let mut map_dist = sweet_locations
                .iter()
                .map(|loc| (Location::from_position(input.me.location).dist(loc), loc))
                .collect::<Vec<_>>();

            map_dist.sort_by(|c0, c1| {
                let c0_dist = c0.0;
                let c1_dist = c1.0;
                if c0_dist == c1_dist {
                    if c0.1.position() < c1.1.position() {
                        Ordering::Less
                    } else if c0.1.position() > c1.1.position() {
                        Ordering::Greater
                    } else {
                        Ordering::Equal
                    }
                } else if 0 <= c0_dist && 0 <= c1_dist {
                    if c0_dist < c1_dist {
                        Ordering::Less
                    } else {
                        Ordering::Greater
                    }
                } else if c0_dist < 0 && 0 <= c1_dist {
                    Ordering::Greater
                } else if c1_dist < 0 && 0 <= c0_dist {
                    Ordering::Less
                } else {
                    if c0_dist < c1_dist {
                        Ordering::Less
                    } else {
                        Ordering::Greater
                    }
                }
            });

            dbg!(&map_dist);

            if 0 < map_dist.len() {
                let mut has_print = false;
                for (dist, location) in map_dist.iter() {
                    if *dist != 0 {
                        println!("MOVE {}", location.position());
                        has_print = true;
                        break;
                    }
                }

                if !has_print {
                    println!("{}", input.possible_moves[0]);
                }
            } else {
                println!("RANDOM");
            }
        } else if input.game_phase == "RELEASE" {
            // let competences = compute_competence(&input);
            // dbg!(&competences);
            dbg!(&input.hand);
            dbg!(&closest_app);
            dbg!(&score_closest_app);
            dbg!(&input.possible_moves);

            if 0 <= score_closest_app {
                println!("RELEASE {}", closest_app.id);
                // println!("{}", input.possible_moves[0]);
            } else {
                println!("WAIT");
            }
        } else {
            println!("RANDOM");
        }
    }
}

// In the first league: RANDOM | MOVE <zoneId> | RELEASE <applicationId> | WAIT; In later leagues:
// | GIVE <cardType> | THROW <cardType> | TRAINING | CODING | DAILY_ROUTINE
// | TASK_PRIORITIZATION <cardTypeToThrow> <cardTypeToTake> | ARCHITECTURE_STUDY
// | CONTINUOUS_DELIVERY <cardTypeToAutomate> | CODE_REVIEW | REFACTORING;
