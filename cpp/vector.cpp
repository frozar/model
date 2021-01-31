#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> v;

    v.clear ();
    v.resize ( 3 );
    v[2] = 20;
    // Print all the vector: v[0], v[1], v[2]
    for (int i = 0 ; i < v.size (); ++i)
    {
        cout << "v[" << i << "]: " << v[i] << endl;
    }

    v.clear ();
    v.reserve (3);
    v[2] = 200;
    v.push_back (100);
    // Print only one element thank to the push back: v[0]
    for (int i = 0 ; i < v.size (); ++i)
    {
        cout << "v[" << i << "]: " << v[i] << endl;
    }
}

// g++ -o vector vector.cpp && ./vector
