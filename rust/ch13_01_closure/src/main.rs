use std::thread;
use std::time::Duration;

// fn simulated_expensive_calculation(intensity: u32) -> u32 {
//     println!("calculating slowly...");
//     thread::sleep(Duration::from_secs(2));
//     intensity
// }

fn main() {
    let simulated_user_specified_value = 10;
    let simulated_random_number = 7;

    generate_workout(
        simulated_user_specified_value,
        simulated_random_number
    );
}

// IMPL 0
// struct Cacher<T>
//     where T: Fn(u32) -> u32
// {
//     calculation: T,
//     value: Option<u32>,
// }

// impl<T> Cacher<T>
//     where T: Fn(u32) -> u32
// {
//     fn new(calculation: T) -> Cacher<T> {
//         Cacher {
//             calculation,
//             value: None,
//         }
//     }

//     fn value(&mut self, arg: u32) -> u32 {
//         match self.value {
//             Some(v) => v,
//             None => {
//                 let v = (self.calculation)(arg);
//                 self.value = Some(v);
//                 v
//             },
//         }
//     }
// }

// IMPL 1
// struct Cacher<T, U>
// where T: Fn(U) -> U,
//       U: Copy,
// {
//     calculation: T,
//     value: Option<U>,
// }

// impl<T, U> Cacher<T, U>
//     where T: Fn(U) -> U,
//           U: Copy,
// {
//     fn new(calculation: T) -> Cacher<T, U> {
//         Cacher {
//             calculation,
//             value: None,
//         }
//     }

//     fn value(&mut self, arg: U) -> U {
//         match self.value {
//             Some(v) => v,
//             None => {
//                 let v = (self.calculation)(arg);
//                 self.value = Some(v);
//                 v
//             },
//         }
//     }
// }

// // IMPL 2
// struct Cacher<T, U>
// where T: Fn(U) -> U,
//       U: Clone,
// {
//     calculation: T,
//     value: Option<U>,
// }

// impl<T, U> Cacher<T, U>
//     where T: Fn(U) -> U,
//           U: Clone,
// {
//     fn new(calculation: T) -> Cacher<T, U> {
//         Cacher {
//             calculation,
//             value: None,
//         }
//     }

//     fn value(&mut self, arg: U) -> U {
//         match self.value.clone() {
//             Some(v) => v,
//             None => {
//                 let v = (self.calculation)(arg);
//                 self.value = Some(v.clone());
//                 v
//             },
//         }
//     }
// }

// IMPL 3
use std::collections::HashMap;
use std::hash::Hash;

struct Cacher<T, U, V>
where T: Fn(U) -> V,
      U: Eq + Hash + Copy,
      V: Copy,
{
    calculation: T,
    value: HashMap<U, V>,
}

impl<T, U, V> Cacher<T, U, V>
where T: Fn(U) -> V,
      U: Eq + Hash + Copy,
      V: Copy,
{
    fn new(calculation: T) -> Cacher<T, U, V> {
        Cacher {
            calculation,
            value: HashMap::new(),
        }
    }

    fn value(&mut self, arg: U) -> V {
        // Two-part match here is forced by borrow checker
        // self.values immutable borrow in the match, mutable borrow in the None arm
        match self.value.get(&arg) {
            Some(v) => return *v,
            None => (),
        }
        let v = (self.calculation)(arg);
        self.value.insert(arg, v);
        v
    }
}

#[test]
fn call_with_different_values() {
    let mut c = Cacher::new(|a| a);

    let v1 = c.value(1);
    let v2 = c.value(2);

    assert_eq!(v2, 2);
}

fn generate_workout(intensity: u32, random_number: u32) {
    let mut expensive_result = Cacher::new(|num| {
        println!("calculating slowly...");
        thread::sleep(Duration::from_secs(2));
        num
    });

    if intensity < 25 {
        println!(
            "Today, do {} pushups!",
            expensive_result.value(intensity)
        );
        println!(
            "Next, do {} situps!",
            expensive_result.value(intensity)
        );
    } else {
        if random_number == 3 {
            println!("Take a break today! Remember to stay hydrated!");
        } else {
            println!(
                "Today, run for {} minutes!",
                expensive_result.value(intensity)
            );
        }
    }
}

fn example_01() {
    let x = vec![1, 2, 3];

    let equal_to_x = move |z| z == x;

    // Because of the `move` keyword, `x` is not accessible anymore
    // println!("can't use x here: {:?}", x);

    let y = vec![1, 2, 3];

    assert!(equal_to_x(y));
}
