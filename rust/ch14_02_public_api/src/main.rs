extern crate art;

// use art::kinds::PrimaryColor;
// use art::utils::mix;

// Don't have to guess the internal structure of the crate
use art::PrimaryColor;
use art::mix;

fn main() {
    let red = PrimaryColor::Red;
    let yellow = PrimaryColor::Yellow;
    mix(red, yellow);
}
