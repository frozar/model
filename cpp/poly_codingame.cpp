#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <math.h>

using namespace std;

const static int LOOTER_COUNT = 3;

// struct A
// {
//   int i;
//   A (int i) {this->i = i;}

//   void p_A(A ob)
//   {
//     cout << "In p_A: ob.i: " << ob.i << endl;
//   }
// };

// struct subA : public A
// {
//   subA (int i) : A(i) {}
// };

// struct subB : public A
// {
//   subB (int i) : A(i) {}
// };

struct Point
{
    double x;
    double y;

    Point() {
        x = 12000;
        y = 12000;
    }

    Point(const double & x, const double & y) {
        this->x = x;
        this->y = y;
    }

    // bool isInRange(const Point & p, const double & range) const {
    //     return (*this != p) && (this->distance(p) <= range);
    // }

    // bool isInRange(const Point * p, const double & range) const {
    //     return (*this != *p) && (this->distance(*p) <= range);
    // }

    bool isInRange(Point * p, const double & range){
        return (*this != *p) && (this->distance(*p) <= range);
    }

    // bool isInRange(Point & p, const double & range){
    //     return (*this != p) && (this->distance(p) <= range);
    // }

    // bool isInRange(Point p, const double & range){
    //     return (*this != p) && (this->distance(p) <= range);
    // }

    double distance(const Point & p) const {
        return sqrt((this->x - p.x) * (this->x - p.x) + (this->y - p.y) * (this->y - p.y));
    }

    inline friend bool operator== (const Point & p0, const Point & p1)
    {
        return ((p0.x == p1.x) && (p0.y == p1.y));
    }


    inline friend bool operator!= (const Point & p0, const Point & p1)
    {
        return !(p0 == p1);
    }
};

struct Unit : public Point
{
    int type;
    int id;
    double vx;
    double vy;
    double radius;
    double mass;
    double friction;
    bool known;

    Unit() {}

    Unit(const int & type, const int & id,
         const double & x, const double & y) : Point(x, y) {
        // this->Point (x, y);
        // this->x = x;
        // this->y = y;

        // id = GLOBAL_ID++;
        this->type = type;
        this->id = id;

        vx = 0.0;
        vy = 0.0;

        this->known = false;
    }
};

struct Player;

struct Looter : public Unit
{
    int skillCost;
    double skillRange;
    bool skillActive;

    // Player * player;

    Point wantedThrustTarget;
    int wantedThrustPower;

    // String message;
    // Action attempt;
    // SkillResult skillResult;

    Looter() {};

    Looter(const int & type, Player * player,
           const double & x, const double & y);
};

struct Reaper;

struct Player {
    int score;
    int index;
    int rage;
    Looter looters[LOOTER_COUNT];
    bool dead;
    // Queue<TankerSpawn> tankers;

    Player() {}

    Player(const int & index);

    Reaper * getReaper();

    // Destroyer * getDestroyer();

    // Doof * getDoof();
};

struct Reaper : public Looter {
    Reaper(Player * player, double x, double y);
};

struct Wreck : public Point {
    int id;
    double radius;
    int water;
    bool known;
    Player player;

    Wreck(double x, double y, int water, double radius) : Point(x, y) {
        // id = GLOBAL_ID++;
        this->radius = radius;
        this->water = water;
    }

    // TO IMPLEMENT
    // Reaper harvesting
    // bool harvest(List<Player> players, Set<SkillEffect> skillEffects) {
        // players.forEach(p -> {
        //     if (isInRange(p.getReaper(), radius) && !p.getReaper().isInDoofSkill(skillEffects)) {
        //         p.score += 1;
        //         water -= 1;
        //     }
        // });
    //     return water > 0;
    // }

    bool harvest(std::vector<Player> & players) {
        for (uint i = 0; i < players.size (); ++i )
        {
            Player & p = players[i];
            if (isInRange(p.getReaper(), radius))
            {
                p.score += 1;
                water -= 1;
            }
        }

        return water > 0;
    }
};

int main()
{
  // subA subA_ob( 3 );
  // cout << "subA_ob.i: " << subA_ob.i << endl;
  // subA_ob.p_A(subA_ob);

  // subB subB_ob( 3 );
  // subB_ob.p_A(&subA_ob);
}

// g++ -o poly_codingame poly_codingame.cpp && ./poly_codingame
// g++ -c poly_codingame.cpp

