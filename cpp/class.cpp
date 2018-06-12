#include <iostream>

template<typename scalarT>
class Parent
{
public:
    Parent (const scalarT);

    void display () const;

protected:
    scalarT internal_;
};

template<typename scalarT>
Parent<scalarT>::Parent (const scalarT internal) :
    internal_ (internal)
{}

template<typename scalarT>
void
Parent<scalarT>::display () const
{
    std::cout << "Internal value: " << internal_ << std::endl;
}

template<typename scalarT>
class Child : public Parent<scalarT>
{
public:
    Child (const scalarT);

    void display () const;

protected:
    using Parent<scalarT>::internal_;
};

template<typename scalarT>
Child<scalarT>::Child (const scalarT internal) :
    Parent<scalarT> (internal)
{}

template<typename scalarT>
void
Child<scalarT>::display () const
{
    std::cout << "From Child, internal value: " << internal_ << std::endl;
}

int main(int argc, char ** argv) {
    // Parent<int> p (5);
    // p.display ();

    Child<int> c (5);
    c.display ();

    return 0;
}

// g++ -o class class.cpp && ./class
