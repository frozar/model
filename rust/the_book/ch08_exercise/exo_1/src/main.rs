extern crate rand;

// use rand::Rng;

fn main() {
    // Generate a random v
    // let mut v = Vec::new();
    // for _ in 1..21 {
    //     let random_int = rand::thread_rng().gen_range(1, 101);
    //     v.push(random_int);
    // }
    let mut v = vec![73, 4, 93, 16, 4, 4, 38, 36, 70, 94, 29, 30, 49, 71, 47, 43, 35, 62, 58, 1];
    println!("&v: {:?}", &v);

    let mut mean = 0;
    for i in &v {
        mean += i;
    }
    let mean = mean as f32 / v.len() as f32;
    println!("mean: {}", mean);

    v.sort();
    println!("v.sort(): {:?}", &v);
    let median = v.len()/2 - 1;
    println!("median index: {}", median);
    let median = v[median];
    println!("median: {}", median);

    // Construct the histogram on the number which appear in the vector v
    use std::collections::HashMap;
    let mut map = HashMap::new();
    for i in &v {
        let count = map.entry(i).or_insert(0);
        *count += 1;
    }

    let mut max_count = 0;
    let mut mode_value = v[0];
    for (key, count) in &map {
        if max_count < *count {
            max_count = *count;
            mode_value = **key;
        }
    }
    println!("max_count: {}", max_count);
    println!("mode_value: {}", mode_value);
}
