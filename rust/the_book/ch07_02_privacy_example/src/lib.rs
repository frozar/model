
mod outermost {
    pub fn middle_function() {}

    fn middle_secret_function() {
        println!("0 In middle_secret_function");
    }

    pub mod inside {
        pub fn inner_function() {
            ::outermost::middle_secret_function();
        }

        fn secret_function() {
            println!("1 In secret_function");
        }

        pub mod subinside {
            pub fn subinner_function() {
                ::outermost::middle_secret_function();
            }

            fn secret_function() {
                println!("2 In secret_function");
            }

            pub mod subsubinside {
                pub fn subsubinner_function() {
                    ::outermost::middle_secret_function();
                    ::outermost::inside::secret_function();
                    ::outermost::inside::subinside::secret_function();
                }
            }

        }
    }
}

pub fn try_me() {
    outermost::middle_function();
    outermost::inside::inner_function();
    outermost::inside::subinside::subinner_function();
    outermost::inside::subinside::subsubinside::subsubinner_function();
}
