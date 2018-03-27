extern crate aggregator;

use aggregator::Summary;

struct WeatherForecast {
    high_temp: f64,
    low_temp: f64,
    chance_of_precipitation: f64,
}

// Implementation of a trait on a local type of an external trait: orphan rule
impl Summary for WeatherForecast {
    fn summarize(&self) -> String {
        format!("The high will be {}, and the low will be {}. The chance of
        precipitation is {}%.", self.high_temp, self.low_temp,
                self.chance_of_precipitation)
    }

    fn summarize_author(&self) -> String {
        format!("precipitation probability {} (no the author name)", self.chance_of_precipitation)
    }
}

// Trait bounds: To be able to call 'summarize()', the generic type have to implement
// the trait Summary, so we have to bound it: 'T: Summary'
pub fn notify<T: Summary>(item: T) {
    println!("Breaking news! {}", item.summarize());
}


use std::fmt::Display;
use std::fmt::Debug;

// The signature can become a bit confuse because of trait bound
fn some_function_sample1<T: Display + Clone, U: Clone + Debug>(t: T, u: U) -> i32 {
    2
}

// In this case it is possible to use the keyword 'where'
fn some_function_sample2<T, U>(t: T, u: U) -> i32
    where T: Display + Clone,
          U: Clone + Debug
{
    2
}

fn main() {
    let tweet = aggregator::Tweet {
        username: String::from("horse_ebooks"),
        content: String::from("of course, as you probably already know, people"),
        reply: false,
        retweet: false,
    };

    println!("1 new tweet: {}", tweet.summarize());
    println!("1 new tweet: {}", tweet.summarize_default());
}
