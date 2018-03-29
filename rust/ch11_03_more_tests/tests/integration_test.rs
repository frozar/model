extern crate adder;

#[test]
fn it_adds_two() {
    assert_eq!(4, adder::add_two(2));
}

// Run only test contained in this file:
// $ cargo test --test integration_test
