use std::fs::File;
use std::io::ErrorKind;

use std::io;
use std::io::Read;

fn main() {
    example_1();
    example_2();
    example_3();
    example_4();
    example_5();
    example_6();
    example_7();
}

fn example_1() {
    let f = File::open("hello.txt");

    let f = match f {
        Ok(file) => file,
        Err(error) => {
            panic!("There was a problem opening the file: {:?}", error)
        },
    };
}

fn example_2() {
    let f = File::open("hello.txt");

    let f = match f {
        Ok(file) => file,
        Err(ref error) if error.kind() == ErrorKind::NotFound => {
            match File::create("hello.txt") {
                Ok(fc) => fc,
                Err(e) => {
                    panic!(
                        "Tried to create file but there was a problem: {:?}",
                        e
                    )
                },
            }
        },
        Err(error) => {
            panic!(
                "There was a problem opening the file: {:?}",
                error
            )
        },
    };
}

fn example_3() {
    let f = File::open("hello.txt").unwrap();
}

fn example_4() {
    let f = File::open("hello.txt").expect("Failed to open hello.txt");
}

fn example_5() {
    let s = match read_username_from_file() {
        Ok(s) => s,
        Err(error) => {
            panic!("There was a problem reading the file: {:?}", error)
        },
    };
    println!("Success to read the file: {:?}", s);
}

fn read_username_from_file() -> Result<String, io::Error> {
    let f = File::open("hello.txt");

    let mut f = match f {
        Ok(file) => file,
        Err(e) => return Err(e),
    };

    let mut s = String::new();

    match f.read_to_string(&mut s) {
        Ok(_) => Ok(s),
        Err(e) => Err(e),
    }
}

fn example_6() {
    let s = match read_username_from_file_2() {
        Ok(s) => s,
        Err(error) => {
            panic!("There was a problem reading the file: {:?}", error)
        },
    };
    println!("Success to read the file: {:?}", s);
}

fn read_username_from_file_2() -> Result<String, io::Error> {
    let mut f = File::open("hello.txt")?;
    let mut s = String::new();
    f.read_to_string(&mut s)?;
    Ok(s)
}

fn example_7() {
    let s = match read_username_from_file_3() {
        Ok(s) => s,
        Err(error) => {
            panic!("There was a problem reading the file: {:?}", error)
        },
    };
    println!("Success to read the file: {:?}", s);
}

fn read_username_from_file_3() -> Result<String, io::Error> {
    let mut s = String::new();

    File::open("hello.txt")?.read_to_string(&mut s)?;

    Ok(s)
}

