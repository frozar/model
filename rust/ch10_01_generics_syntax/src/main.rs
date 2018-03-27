fn main() {
    example_01();
    example_02();
    example_03();
    example_04();
    example_05();
    example_06();
}

fn largest_i32(list: &[i32]) -> i32 {
    let mut largest = list[0];

    for &item in list.iter() {
        if item > largest {
            largest = item;
        }
    }

    largest
}

fn largest_char(list: &[char]) -> char {
    let mut largest = list[0];

    for &item in list.iter() {
        if item > largest {
            largest = item;
        }
    }

    largest
}

fn example_01() {
    let number_list = vec![34, 50, 25, 100, 65];

    let result = largest_i32(&number_list);
    println!("The largest number is {}", result);

    let char_list = vec!['y', 'm', 'a', 'q'];

    let result = largest_char(&char_list);
    println!("The largest number is {}", result);
}

// The Copy trait does the allocation on the stack
fn largest<T: PartialOrd + Copy>(list: &[T]) -> T {
    let mut largest = list[0];

    for &item in list.iter() {
        if item > largest {
            largest = item;
        }
    }

    largest
}

// The Clone trait could do allocation on the heap if the input type
// does regularly allocation on the heap as 'String' does.
fn largest_maybe_on_heap<T: PartialOrd + Clone>(list: &[T]) -> T {
    let mut largest = list[0].clone();

    for item in list.iter() {
        if *item > largest {
            largest = (*item).clone();
        }
    }

    // alternative implementation
    // for ref item in list.iter() {
    //     if **item > largest {
    //         largest = (**item).clone();
    //     }
    // }

    largest
}

// Without Copy or Clone trait
fn largest_without<T: PartialOrd>(list: &[T]) -> &T {
    let mut largest = &list[0];

    for item in list.iter() {
        if *item > *largest {
            largest = item;
        }
    }

    largest
}

fn example_02() {
    let number_list = vec![34, 50, 25, 100, 65];

    let result = largest(&number_list);
    println!("The largest number is {}", result);

    let char_list = vec!['y', 'm', 'a', 'q'];

    let result = largest(&char_list);
    println!("The largest char is {}", result);
}

struct Point<T> {
    x: T,
    y: T,
}

fn example_03() {
    let integer = Point { x: 5, y: 10 };
    let float = Point { x: 1.0, y: 4.0 };
}

struct Point_2<T, U> {
    x: T,
    y: U,
}

fn example_04() {
    let both_integer = Point_2 { x: 5, y: 10 };
    let both_float = Point_2 { x: 1.0, y: 4.0 };
    let integer_and_float = Point_2 { x: 5, y: 4.0 };
}

impl<T> Point<T> {
    fn x(&self) -> &T {
        &self.x
    }
}

fn example_05() {
    let p = Point { x: 5, y: 10 };

    println!("p.x = {}", p.x());
}

impl Point<f32> {
    fn distance_from_origin(&self) -> f32 {
        (self.x.powi(2) + self.y.powi(2)).sqrt()
    }
}

impl<T, U> Point_2<T, U> {
    fn mixup<V, W>(self, other: Point_2<V, W>) -> Point_2<T, W> {
        Point_2 {
            x: self.x,
            y: other.y,
        }
    }
}

fn example_06() {
    let p1 = Point_2 { x: 5, y: 10.4 };
    let p2 = Point_2 { x: "Hello", y: 'c'};

    let p3 = p1.mixup(p2);

    println!("p3.x = {}, p3.y = {}", p3.x, p3.y);
}
