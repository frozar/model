class MyClass_A:
    """A simple example class"""
    i = 12345

    def f(self):
        return 'hello world'

class MyClass_B:
    """A simple example class B"""
    i = "toto"

    def f(self):
        return 'hello world'

    def example(self):
        a = MyClass_A()
        print "in example a.f: ", a.f()
        
toto = MyClass_B()
toto.example()
