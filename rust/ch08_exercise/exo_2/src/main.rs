fn main() {
    println!("Write a word");

    let mut input_word = String::new();

    std::io::stdin().read_line(&mut input_word)
        .expect("Failed to read line");

    let input_word = input_word.trim();
    if input_word.is_empty() {
        println!("Nothing as input");
        std::process::exit(0);
    }

    let first_letter = input_word.chars().next().unwrap();

    let vowels = ['a', 'e', 'i', 'o', 'u', 'y'];

    let output_word;
    if vowels.contains(&first_letter) {
        output_word = format!("{}-{}", input_word, "hay");
    }
    else {
        let mut tmp_buf = String::new();
        let mut chars = input_word.chars();
        // Skip the first letter
        chars.next();

        for ch in chars {
            tmp_buf.push(ch);
        }

        if tmp_buf.is_empty() {
            output_word = format!("{}{}", first_letter, "ay");
        }
        else {
            output_word = format!("{}-{}{}", tmp_buf, first_letter, "ay");
        }
    }

    println!("output_word: {}", output_word);
}
