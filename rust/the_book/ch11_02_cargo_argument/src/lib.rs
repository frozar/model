pub fn add_two(a: i32) -> i32 {
    a + 2
}


fn prints_and_returns_10(a: i32) -> i32 {
    println!("I got the value {}", a);
    10
}

#[cfg(test)]
mod tests {
    use super::*;


    #[test]
    fn this_test_will_pass() {
        let value = prints_and_returns_10(4);
        assert_eq!(10, value);
    }

    #[test]
    fn this_test_will_fail() {
        let value = prints_and_returns_10(8);
        assert_eq!(5, value);
    }

    #[test]
    fn add_two_and_two() {
        assert_eq!(4, add_two(2));
    }

    #[test]
    fn add_three_and_two() {
        assert_eq!(5, add_two(3));
    }

    #[test]
    fn one_hundred() {
        assert_eq!(102, add_two(100));
    }

    // The `#[ignore]` flag disable the run of a test by default
    #[test]
    #[ignore]
    fn expensive_test() {
        // code that takes an hour to run
    }
}

// OPTION FOR CARGO TEST COMMAND
// Run test sequentially:
// $ cargo test -- --test-threads=1

// Display output even if a test pass
// $ cargo test -- --nocapture

// Select a test to run
// $ cargo test one_hundred

// It is possible to run the test for a given module
// $ cargo test tests

// Run tests that are marked as `#[ignore]`
// $ cargo test -- --ignored
