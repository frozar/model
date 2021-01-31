use std::io;

// macro_rules! parse_input {
//     ($x:expr, $t:ident) => {
//         $x.trim().parse::<$t>().unwrap()
//     };
// }

#[derive(Debug)]
enum Operand {
    ParenOpen,
    ParenClose,
    Addition,
    Substraction,
    Multiplication,
    Division,
    Inferior,
    Superior,
}

impl Operand {
    fn new(operand: &char) -> Option<Operand> {
        use Operand::*;

        match *operand {
            '(' => Some(ParenOpen),
            ')' => Some(ParenClose),
            '+' => Some(Addition),
            '-' => Some(Substraction),
            '*' => Some(Multiplication),
            '/' => Some(Division),
            '>' => Some(Superior),
            '<' => Some(Inferior),
            _ => None,
        }
    }

    fn priority(&self) -> i32 {
        use Operand::*;

        match &self {
            ParenOpen => 4,
            ParenClose => 4,
            Division => 3,
            Multiplication => 3,
            Addition => 2,
            Substraction => 2,
            Superior => 1,
            Inferior => 1,
        }
    }
}

#[derive(Debug)]
enum TreeNode {
    // Expression(&'static str),
    Operation(Operand),
    Value(f64),
}

#[derive(Debug)]
enum Tree {
    Cons(TreeNode, Box<Tree>, Box<Tree>),
    Nil,
}

impl Tree {
    // Create an empty list
    fn new() -> Tree {
        // `Nil` has type `Tree`
        Tree::Nil
    }

    // // Consume a list, and return the same list with a new element at its front
    // fn prepend(self, elem: u32) -> Tree {
    //     // `Cons` also has type List
    //     Cons(elem, Box::new(self))
    // }

    fn appendLeft(&mut self, node: TreeNode) {
        let t_cons = Tree::Cons(node, Box::new(Tree::new()), Box::new(Tree::new()));

        match self {
            Tree::Cons(_t_node, ref mut left, _right) => left = Box::new(t_cons),
            Tree::Nil => *self = t_cons,
        }
    }
}

fn main() {
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let expr = input_line.trim_right().to_string();
    eprintln!("{:?}", expr);

    let mut v: Vec<(usize, Operand, i32)> = Vec::new();
    for (i, c) in expr.chars().enumerate() {
        if let Some(op) = Operand::new(&c) {
            let prio = op.priority();
            // eprintln!("op {:?}", op);
            // eprintln!("prio {:?}", prio);
            // eprintln!("op.prio {:?}", op.priority());
            v.push((i, op, prio));
        }
    }

    let mut t = Tree::new();

    let t_node_0 = TreeNode::Value(1.);
    let t_cons_0 = Tree::Cons(t_node_0, Box::new(Tree::new()), Box::new(Tree::new()));
    
    let t_node = TreeNode::Operation(Operand::Addition);
    eprintln!("t_node {:?}", t_node);
    let t_cons = Tree::Cons(t_node, Box::new(Tree::new()), Box::new(Tree::new()));
    match &t_cons {
        // Tree::Cons(ref t_node, ref left, ref right) =>
        Tree::Cons(t_node, _left, _right) =>
            eprintln!("t_cons.0 {:?}", t_node),
        Tree::Nil => ()
    }
    // eprintln!("t_cons {:?}", t_cons);
    t = t_cons;
    eprintln!("t {:?}", t);

    eprintln!("{:?}", v);

    println!("6 100.00");
}
