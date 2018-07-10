pub struct Post {
    state: Option<Box<State>>,
    content: String,
}

impl Post {
    pub fn new() -> Post {
        Post {
            state: Some(Box::new(Draft {})),
            content: String::new(),
        }
    }

    pub fn add_text(&mut self, text: &str) {
        self.content.push_str(text);
        // let wk_text = self.state.as_ref().unwrap().add_text(&text, self);
        // self.content.push_str(wk_text);
    }
    
    pub fn content(&self) -> &str {
        self.state.as_ref().unwrap().content(&self)
    }

    pub fn request_review(&mut self) {
        if let Some(s) = self.state.take() {
            self.state = Some(s.request_review())
        }
    }

    pub fn approve(&mut self) {
        if let Some(s) = self.state.take() {
            self.state = Some(s.approve())
        }
    }

    pub fn reject(&mut self) {
        if let Some(s) = self.state.take() {
            self.state = Some(s.reject())
        }
    }
}

trait State {
    fn request_review(self: Box<Self>) -> Box<State>;
    fn approve(self: Box<Self>) -> Box<State>;

    fn reject(self: Box<Self>) -> Box<State>;

    fn content<'a>(&self, _post: &'a Post) -> &'a str {
        ""
    }

    // fn add_text<'a>(&self, text: &'a str, post: &'a mut Post) -> &'a str {
    //     post.content()
    // }
}

struct Draft {}

impl State for Draft {
    fn request_review(self: Box<Self>) -> Box<State> {
        Box::new(PendingReview::new())
    }

    fn approve(self: Box<Self>) -> Box<State> {
        self
    }

    fn reject(self: Box<Self>) -> Box<State> {
        self
    }

    // fn add_text<'a>(&self, text: &'a str, post: &'a mut Post) -> &'a str {
    //     text
    // }
}

struct PendingReview {
    nb_approved: i32,
}

impl PendingReview {
    pub fn new() -> PendingReview {
        PendingReview {
            nb_approved: 0,
        }
    }
}


impl State for PendingReview {
    fn request_review(self: Box<Self>) -> Box<State> {
        self
    }

    fn approve(mut self: Box<Self>) -> Box<State> {
        self.nb_approved += 1;
        if self.nb_approved <= 1 {
            self
        }
        else {
            Box::new(Published {})
        }
    }

    fn reject(self: Box<Self>) -> Box<State> {
        Box::new(Draft {})
    }
}

struct Published {}

impl State for Published {
    fn request_review(self: Box<Self>) -> Box<State> {
        self
    }

    fn approve(self: Box<Self>) -> Box<State> {
        self
    }

    fn reject(self: Box<Self>) -> Box<State> {
        self
    }

    fn content<'a>(&self, post: &'a Post) -> &'a str {
        &post.content
    }
}
