use std::io;

macro_rules! parse_input {
    ($x:expr, $t:ident) => {
        $x.trim().parse::<$t>().unwrap()
    };
}

#[derive(Debug, PartialEq, Copy, Clone)]
struct City {
    id: i32,
    x: i32,
    y: i32,
}

fn parse_input() -> Vec<City> {
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let n = parse_input!(input_line, i32);
    let mut cities: Vec<City> = vec![];
    for i in 0..n as usize {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        let inputs = input_line.split(" ").collect::<Vec<_>>();
        let x = parse_input!(inputs[0], i32);
        let y = parse_input!(inputs[1], i32);
        cities.push(City {
            id: i as i32,
            x: x,
            y: y,
        });
    }

    return cities;
}

fn pop<T: Copy>(vector: &mut Vec<T>, index: usize) -> T {
    let one_item = vector.drain(index..(index + 1)).collect::<Vec<_>>();
    let item = *one_item.first().unwrap();

    return item;
}

fn get_city<'a>(city_id: &'a i32, cities: &'a Vec<City>) -> &'a City {
    let mut res: &City = cities.first().unwrap();
    for city in cities.iter() {
        if (city.id == *city_id) {
            res = city;
            break;
        }
    }
    return res;
}

fn dist(city0: &City, city1: &City) -> f64 {
    let x = city0.x - city1.x;
    let y = city0.y - city1.y;
    return ((x * x + y * y) as f64).sqrt();
}

fn nearest_city(
    current_city: &City,
    cities: &Vec<City>,
    consumed_cities: &Vec<i32>,
) -> (City, f64) {
    let mut res = *cities.first().unwrap();
    let mut dist_min = f64::MAX;
    for city in cities.iter() {
        if (consumed_cities.contains(&city.id)) {
            continue;
        }

        let dist = dist(current_city, city);
        if (dist < dist_min) {
            dist_min = dist;
            res = *city;
        }
    }
    return (res, dist_min);
}

fn compute_travelling(cities: &Vec<City>) -> i32 {
    let mut consumed_cities: Vec<i32> = vec![];
    let mut cities_to_visit: Vec<i32> = cities.iter().map(|city| city.id).collect::<Vec<_>>();
    // eprintln!("{:?}", cities_to_visit);
    let mut current_city_id = pop(&mut cities_to_visit, 0);
    consumed_cities.push(current_city_id);
    let mut current_city = get_city(&current_city_id, &cities);
    // eprintln!("{}", current_city_id);
    // eprintln!("{:?}", current_city);

    let mut total_dist = 0.0_f64;
    while !cities_to_visit.is_empty() {
        let (next_city, dist_increment) = nearest_city(current_city, &cities, &consumed_cities);
        // eprintln!("{:?} {:?}", next_city, dist_increment);
        total_dist += dist_increment;
        let index_next_city = cities_to_visit
            .iter()
            .position(|&i| i == next_city.id)
            .unwrap();
        // eprintln!("index_next_city {}", index_next_city);

        current_city_id = pop(&mut cities_to_visit, index_next_city);
        consumed_cities.push(current_city_id);
        current_city = get_city(&current_city_id, &cities);
        // eprintln!("{}", current_city_id);
        // eprintln!("{:?}", current_city);
    }
    let last_dist = dist(current_city, cities.first().unwrap());
    total_dist += last_dist;
    return total_dist.round() as i32;
}

fn main() {
    let mut cities = parse_input();
    // for city in cities.iter() {
    //     eprintln!("{:?}", city);
    // }
    let answer = compute_travelling(&cities);

    println!("{}", answer);
}
