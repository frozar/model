#include <cstdio>
#include <map>
#include <cstring>
#include <sstream>
#include <vector>
using namespace std;

int T;
map <int, bool> hap [11];
char line [1000];

inline bool happy (int b, int x)
{
    if (x == 1)
        return true;

    if (x == 0)
        return false;

    if (hap [b].find (x) != hap [b].end ())
        return hap [b][x];

    bool &ans = hap [b][x];
    int sum = 0;

    while (x > 0)
    {
        sum += (x % b) * (x % b);
        x /= b;
    }

    ans = false;
    return ans = happy (b, sum);
}

int main ()
{
    scanf ("%d", &T);

    for (int t = 1; t <= T; t++)
    {
        do
            gets (line);
        while (strlen (line) == 0);

        istringstream in (line);
        int x;
        vector <int> bases;

        while (in >> x)
            bases.push_back (x);

        int best = 2;

        while (true)
        {
            bool good = true;

            for (int i = 0; i < (int) bases.size () && good; i++)
                if (!happy (bases [i], best))
                    good = false;

            if (good)
            {
                printf ("Case #%d: %d\n", t, best);
                break;
            }

            best++;
        }
    }

    return 0;
}
