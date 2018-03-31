use std::fs::File;
// use std::io::prelude::*;
use std::io::Read;
use std::error::Error;

pub struct Config {
    pub query: String,
    pub filename: String,
}

impl Config {
    pub fn new(args: &[String]) -> Result<Config, &'static str> {
        if args.len() < 3 {
            return Err("not enough arguments");
        }
        let query = args[1].clone();
        let filename = args[2].clone();

        Ok(Config{ query, filename })
    }
}

pub fn run(config: Config) -> Result<(), Box<Error>> {
    let mut f = File::open(config.filename).expect("file not found");

    let mut contents = String::new();
    f.read_to_string(&mut contents)?;

    for line in search(&config.query, &contents) {
        println!("{}", line);
    }

    Ok(())
}

pub fn search<'a>(query: &str, contents: &'a str) -> Vec<&'a str> {
    let mut results = Vec::new();

    for line in contents.lines() {
        if line.contains(query) {
            results.push(line);
        }
    }

    results
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn config_new_not_enough_arg() {
        let arg = ["0".to_string(), "1".to_string()];
        let config = Config::new(&arg);
        assert!(config.is_err());
    }

    #[test]
    fn config_new_good_value() {
        let arg = ["program_name".to_string(), "0".to_string(), "1".to_string()];
        let config = Config::new(&arg).unwrap();
        assert_eq!(config.query, "0");
        assert_eq!(config.filename, "1");
    }

    #[test]
    #[should_panic(expected = "file not found")]
    fn run_cant_open_file() {
        let arg = ["program_name".to_string(), "0".to_string(), "data/toto.txt".to_string()];
        let config = Config::new(&arg).unwrap();
        run(config).unwrap();
    }

    #[test]
    fn one_result() {
        let query = "duct";
        let contents = "\
Rust:
safe, fast, productive.
Pick three.";

        assert_eq!(
            vec!["safe, fast, productive."],
            search(query, contents)
        );
    }
}
