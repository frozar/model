#include <iostream>
#include <vector>
#include <memory>

struct myStruct {
    myStruct () {}

    myStruct (const int i)
    {
	t = std::make_unique<int>(i);
    }
    std::unique_ptr<int> t;
};

class Foo
{
public:
    void Add(myStruct& petit_nom)
    {
	m_tata[0].t = std::move(petit_nom.t);
    }

    myStruct m_tata[3]; 
};

int main(int argc, char ** argv) {
    Foo foo;

    myStruct var0(5);
    std::cout << "var0.t         : " << *(var0.t) << std::endl;
    foo.Add(var0);
    std::cout << "foo.m_tata[0].t: " << *(foo.m_tata[0].t) << std::endl;

    myStruct var2(2);
    std::cout << "var2.t         : " << *(var2.t) << std::endl;
    foo.Add(var2);
    std::cout << "foo.m_tata[0].t: " << *(foo.m_tata[0].t) << std::endl;

    return 0;
}

// g++ -o ownership ownership.cpp && ./ownership
