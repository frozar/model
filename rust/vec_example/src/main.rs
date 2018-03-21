fn main() {
    vec_manip();
    example_1();
    example_2();
    example_3();
}

fn vec_manip() {
    let _v: Vec<i32> = Vec::new();
    let _v = vec![1, 2, 3];

    let mut v = Vec::new();

    v.push(5);
    v.push(6);
    v.push(7);
    v.push(8);
    println!("v: {:?}", v);
}

fn example_1() {
    let v = vec![1, 2, 3, 4, 5];

    let _third: &i32 = &v[2];
    let third: Option<&i32> = v.get(2);
    // The following line will not compile: index 100
    // let does_not_exist = &v[100];
    let does_not_exist = v.get(100);
    println!("third: {:?}", third);
    println!("does_not_exist: {:?}", does_not_exist);
}

fn example_2() {
    let v = vec![100, 32, 57];
    for i in &v {
        println!("{}", i);
    }

    let mut v = vec![100, 32, 57];
    for i in &mut v {
        *i += 50;
    }
    println!("v: {:?}", v);
}

fn example_3() {
    #[derive(Debug)]
    enum SpreadsheetCell {
        Int(i32),
        Float(f64),
        Text(String),
    }

    let row = vec![
        SpreadsheetCell::Int(3),
        SpreadsheetCell::Text(String::from("blue")),
        SpreadsheetCell::Float(10.12),
    ];
    println!("row: {:?}", row);
}
