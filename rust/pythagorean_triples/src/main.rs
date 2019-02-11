extern crate nalgebra as na;
use na::{Vector3, RowVector3};
// use na::{Vector9};
use nalgebra::dimension::{U1,U3,U9};

type MyVector1<N> = na::VectorN<N, U1>;
// /// A stack-allocated, column-major, 1x2 matrix.
// pub type Matrix1x2<N> = MatrixMN<N, U1, U2>;
type Matrix9x3<N> = na::MatrixMN<N, U9, U3>;


fn main() {
    let a = na::RowVector3::new(1, 1, 1);
    let b = Vector3::new(1, 1, 1);

    let c = a * b;

    println!("{:?}", c);

    // let _d = Matrix9x3::from_iterator_generic(na::U9, na::U3,
    //                                           vec![-1, 2, 2,
    //                                                -2, 1, 2,
    //                                                -2, 2, 3,
    //                                                1, 2, 2,
    //                                                2, 1, 2,
    //                                                2, 2, 3,
    //                                                1, -2, 2,
    //                                                2, -1, 2,
    //                                                2, -2, 3]);

    let _d = Matrix9x3::from_iterator_generic(na::U9, na::U3,
    vec![-1, 
         -2, 
         -2, 
          1, 
          2, 
          2, 
          1, 
          2, 
          2, 
          2, 
          1, 
          2, 
          2, 
          1, 
          2, 
         -2, 
         -1, 
         -2,
          2,  
          2,  
          3,  
          2,  
          2,  
          3,  
          2,  
          2,  
          3]);

    println!("{:?}", _d[(1, 2)]);
    

    let b = Vector3::new(3, 4, 5);
    let mut a = _d * b; //Vector9::<i32>::zeros();

    // let mut i = 0;
    for _ in 0..30000{
        a = _d * b;
        // i +=1;
    }
    
    println!("{:?}", a);
}
