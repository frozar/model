// #pragma GCC optimize("O3")
// #pragma GCC optimize("inline")
// #pragma GCC optimize("omit-frame-pointer")

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <math.h>

// chrono
#include <chrono>

// printf
#include <stdio.h>

#define now chrono::system_clock::now

using namespace std;

const static int LOOTER_COUNT = 3;

const static double MAP_RADIUS = 6000.0;
const static double WATERTOWN_RADIUS = 3000.0;

const static int TANKER_THRUST = 500;
const static double TANKER_EMPTY_MASS = 2.5;
const static double TANKER_MASS_BY_WATER = 0.5;
const static double TANKER_FRICTION = 0.40;
const static double TANKER_RADIUS_BASE = 400.0;
const static double TANKER_RADIUS_BY_SIZE = 50.0;
const static int TANKER_EMPTY_WATER = 1;
const static int TANKER_MIN_SIZE = 4;
const static int TANKER_MAX_SIZE = 10;
const static double TANKER_MIN_RADIUS = TANKER_RADIUS_BASE + TANKER_RADIUS_BY_SIZE * TANKER_MIN_SIZE;
const static double TANKER_MAX_RADIUS = TANKER_RADIUS_BASE + TANKER_RADIUS_BY_SIZE * TANKER_MAX_SIZE;
const static double TANKER_SPAWN_RADIUS = 8000.0;
const static int TANKER_START_THRUST = 2000;

const static double REAPER_MASS = 0.5;
const static double REAPER_FRICTION = 0.20;
const static int REAPER_SKILL_DURATION = 3;
const static int REAPER_SKILL_COST = 30;
const static int REAPER_SKILL_ORDER = 0;
const static double REAPER_SKILL_RANGE = 2000.0;
const static double REAPER_SKILL_RADIUS = 1000.0;
const static double REAPER_SKILL_MASS_BONUS = 10.0;

const static double DESTROYER_MASS = 1.5;
const static double DESTROYER_FRICTION = 0.30;
const static int DESTROYER_SKILL_DURATION = 1;
const static int DESTROYER_SKILL_COST = 60;
const static int DESTROYER_SKILL_ORDER = 2;
const static double DESTROYER_SKILL_RANGE = 2000.0;
const static double DESTROYER_SKILL_RADIUS = 1000.0;
const static int DESTROYER_NITRO_GRENADE_POWER = 1000;

const static double DOOF_MASS = 1.0;
const static double DOOF_FRICTION = 0.25;
const static double DOOF_RAGE_COEF = 1.0 / 100.0;
const static int DOOF_SKILL_DURATION = 3;
const static int DOOF_SKILL_COST = 30;
const static int DOOF_SKILL_ORDER = 1;
const static double DOOF_SKILL_RANGE = 2000.0;
const static double DOOF_SKILL_RADIUS = 1000.0;

const static double LOOTER_RADIUS = 400.0;
const static int LOOTER_REAPER = 0;
const static int LOOTER_DESTROYER = 1;
const static int LOOTER_DOOF = 2;

const static int TYPE_TANKER = 3;
const static int TYPE_WRECK = 4;
const static int TYPE_REAPER_SKILL_EFFECT = 5;
const static int TYPE_DOOF_SKILL_EFFECT = 6;
const static int TYPE_DESTROYER_SKILL_EFFECT = 7;

const static double EPSILON = 0.00001;
const static double MIN_IMPULSE = 30.0;
const static double IMPULSE_COEFF = 0.5;

const static bool REAPER_SKILL_ACTIVE = true;
const static bool DESTROYER_SKILL_ACTIVE = true;
const static bool DOOF_SKILL_ACTIVE = true;

const static int MAX_THRUST = 300;
const static int MAX_RAGE = 300;
const static int WIN_SCORE = 50;


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

    double distance(const Point & p) const {
        return sqrt((this->x - p.x) * (this->x - p.x) + (this->y - p.y) * (this->y - p.y));
    }

    // Move the point to x and y
    void move(const double & x, const double & y) {
        this->x = x;
        this->y = y;
    }

    // Move the point to an other point for a given distance
    void moveTo(const Point & p, const double & distance) {
        double d = this->distance(p);

        if (d < EPSILON) {
            return;
        }

        double dx = p.x - x;
        double dy = p.y - y;
        double coef = distance / d;

        this->x += dx * coef;
        this->y += dy * coef;
    }

    bool isInRange(const Point & p, const double & range) const {
        return (*this != p) && (this->distance(p) <= range);
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

const static Point WATERTOWN(0, 0);
const static Point NULL_POINT(12000, 12000);

// Forward class declaration
struct Collision;
struct SkillEffect;

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

    Unit(int type, int id,
         double x, double y, double vx, double vy) : Point(x, y) {
        this->type = type;
        this->id = id;

        this->vx = vx;
        this->vy = vy;

        this->known = false;
        // cerr << "UNIT id: " << id << endl;
        // cerr << "UNIT vx: " << this->vx << endl;
        // cerr << "UNIT vy: " << this->vy << endl;
        // cerr << "UNIT end" << endl;
    }

    static Unit * make_unit(int id, int type, int player_id, int size, double x, double y, double vx, double vy);

    void move(const double & t);

    double speed() const;

    void thrust(const Point & p, const int & power);

    bool isInDoofSkill(std::vector<SkillEffect> & skillEffects);

    void adjust(std::vector<SkillEffect> & skillEffects);

    // Search the next collision with the map border
    virtual
    Collision getCollision(bool verbose = false);

    // Search the next collision with an other unit
    Collision getCollision(Unit & u, bool verbose = false);

    // Bounce between 2 units
    void bounce(Unit & u);

    // Bounce with the map border
    void bounce();

    friend std::ostream &
    operator<<(std::ostream& os, const Unit& u)
    {
        os << "unit: "
           << u.id << " "
           << u.x << " " << u.y << " "
           << u.vx << " " << u.vy << " "
           << u.radius << " " << u.mass << " " << u.friction;
        return os;
    }

};

// const static Unit NULL_UNIT (-1, -1, 12000, 12000);
// static Unit NULL_UNIT (-1, -1, 12000, 12000, 0, 0);
static Unit * NULL_UNIT = NULL;

struct Tanker;

struct Collision {
    double t;
    Unit * a;
    Unit * b;

    Collision() {}

    Collision(const double & t) {
        this->t = t;
        // this->a = &NULL_UNIT;
        // this->b = &NULL_UNIT;
        this->a = NULL_UNIT;
        this->b = NULL_UNIT;
    }

    Collision(const double & t, Unit * a) {
        this->t = t;
        this->a = a;
        // this->b = &NULL_UNIT;
        this->b = NULL_UNIT;
    }

    Collision(const double & t, Unit * a, Unit * b) {
        this->t = t;
        this->a = a;
        this->b = b;
    }

    Tanker * dead();
};


void Unit::move(const double & t) {
    x += vx * t;
    y += vy * t;
}

double Unit::speed() const {
    return sqrt(vx * vx + vy * vy);
}

void Unit::thrust(const Point & p, const int & power) {
    double distance = this->distance(p);

    // Avoid a division by zero
    if (fabs(distance) <= EPSILON) {
        return;
    }

    double coef = (((double) power) / mass) / distance;
    vx += (p.x - this->x) * coef;
    vy += (p.y - this->y) * coef;
}

// The null collision 
const static Collision NULL_COLLISION(1.0 + EPSILON);

// Search the next collision with the map border
Collision Unit::getCollision(bool verbose) {
    // Check instant collision
    // cerr << "getCollision" << endl;
    if (verbose) cerr << "this: " << *this << endl;
    if (verbose)  cerr << "distance(WATERTOWN) + radius: " << distance(WATERTOWN) + radius << endl;
    if (distance(WATERTOWN) + radius >= MAP_RADIUS) {
        return Collision(0.0, this);
    }

    // We are not moving, we can't reach the map border
    if (vx == 0.0 && vy == 0.0) {
        return NULL_COLLISION;
    }

    // Search collision with map border
    // Resolving: sqrt((x + t*vx)^2 + (y + t*vy)^2) = MAP_RADIUS - radius <=> t^2*(vx^2 + vy^2) + t*2*(x*vx + y*vy) + x^2 + y^2 - (MAP_RADIUS - radius)^2 = 0
    // at^2 + bt + c = 0;
    // a = vx^2 + vy^2
    // b = 2*(x*vx + y*vy)
    // c = x^2 + y^2 - (MAP_RADIUS - radius)^2

    double a = vx * vx + vy * vy;

    if (a <= 0.0) {
        return NULL_COLLISION;
    }

    double b = 2.0 * (x * vx + y * vy);
    double c = x * x + y * y - (MAP_RADIUS - radius) * (MAP_RADIUS - radius);
    double delta = b * b - 4.0 * a * c;

    if (delta <= 0.0) {
        return NULL_COLLISION;
    }

    double t = (-b + sqrt(delta)) / (2.0 * a);
    // cerr << "getCollision border t: " << t << endl;

    if (t <= 0.0) {
        return NULL_COLLISION;
    }

    return Collision(t, this);
}

// Search the next collision with an other unit
Collision Unit::getCollision(Unit & u, bool verbose) {
    if ( verbose ) cerr << "IN getcollision other" << endl;

    // if ( verbose ) cerr << "PASS 0" << endl;
    
    // Check instant collision
    if (distance(u) <= radius + u.radius) {
        if ( verbose ) cerr << "Instant collision" << endl;
        if ( verbose ) cerr << "distance(u): " << distance(u) << endl;
        if ( verbose ) cerr << "radius: " << radius << endl;
        if ( verbose ) cerr << "u.radius: " << u.radius << endl;
        return Collision(0.0, this, &u);
    }

    // if ( verbose ) cerr << "PASS 1" << endl;

    // Both units are motionless
    if (vx == 0.0 && vy == 0.0 && u.vx == 0.0 && u.vy == 0.0) {
        return NULL_COLLISION;
    }

    // if ( verbose ) cerr << "PASS 2" << endl;

    // Change referencial
    // Unit u is not at point (0, 0) with a speed vector of (0, 0)
    double x2 = x - u.x;
    double y2 = y - u.y;
    double r2 = radius + u.radius;
    double vx2 = vx - u.vx;
    double vy2 = vy - u.vy;

    // Resolving: sqrt((x + t*vx)^2 + (y + t*vy)^2) = radius <=> t^2*(vx^2 + vy^2) + t*2*(x*vx + y*vy) + x^2 + y^2 - radius^2 = 0
    // at^2 + bt + c = 0;
    // a = vx^2 + vy^2
    // b = 2*(x*vx + y*vy)
    // c = x^2 + y^2 - radius^2 

    double a = vx2 * vx2 + vy2 * vy2;

    if (a <= 0.0) {
        return NULL_COLLISION;
    }

    // if ( verbose ) cerr << "PASS 3" << endl;

    double b = 2.0 * (x2 * vx2 + y2 * vy2);
    double c = x2 * x2 + y2 * y2 - r2 * r2;
    double delta = b * b - 4.0 * a * c;

    if (delta < 0.0) {
        return NULL_COLLISION;
    }

    double t = (-b - sqrt(delta)) / (2.0 * a);
    if ( verbose ) cerr << "getCollision other t: " << t << endl;

    if (t <= 0.0) {
        return NULL_COLLISION;
    }

    return Collision(t, this, &u);
}

// Bounce between 2 units
void Unit::bounce(Unit & u) {
    double mcoeff = (mass + u.mass) / (mass * u.mass);
    double nx = x - u.x;
    double ny = y - u.y;
    double nxnysquare = nx * nx + ny * ny;
    double dvx = vx - u.vx;
    double dvy = vy - u.vy;
    double product = (nx * dvx + ny * dvy) / (nxnysquare * mcoeff);
    double fx = nx * product;
    double fy = ny * product;
    double m1c = 1.0 / mass;
    double m2c = 1.0 / u.mass;

    vx -= fx * m1c;
    vy -= fy * m1c;
    u.vx += fx * m2c;
    u.vy += fy * m2c;

    fx = fx * IMPULSE_COEFF;
    fy = fy * IMPULSE_COEFF;

    // Normalize vector at min or max impulse
    double impulse = sqrt(fx * fx + fy * fy);
    double coeff = 1.0;
    // if ((impulse > EPSILON) && (impulse < MIN_IMPULSE)) {
    if ((EPSILON < impulse) && (impulse < MIN_IMPULSE)) {
        coeff = MIN_IMPULSE / impulse;
    }

    // cerr << "IN bounce impulse: " << impulse << endl;
    // cerr << "IN bounce coeff: " << coeff << endl;

    fx = fx * coeff;
    fy = fy * coeff;

    vx -= fx * m1c;
    vy -= fy * m1c;
    u.vx += fx * m2c;
    u.vy += fy * m2c;

    double diff = (distance(u) - radius - u.radius) / 2.0;
    if (diff <= 0.0) {
        // Unit overlapping. Fix positions.
        moveTo(u, diff - EPSILON);
        u.moveTo(*this, diff - EPSILON);
    }
}

// Bounce with the map border
void Unit::bounce() {
    double mcoeff = 1.0 / mass;
    double nxnysquare = x * x + y * y;
    double product = (x * vx + y * vy) / (nxnysquare * mcoeff);
    double fx = x * product;
    double fy = y * product;

    vx -= fx * mcoeff;
    vy -= fy * mcoeff;

    fx = fx * IMPULSE_COEFF;
    fy = fy * IMPULSE_COEFF;

    // Normalize vector at min or max impulse
    double impulse = sqrt(fx * fx + fy * fy);
    double coeff = 1.0;
    if ((impulse > EPSILON) && (impulse < MIN_IMPULSE)) {
        coeff = MIN_IMPULSE / impulse;
    }

    fx = fx * coeff;
    fy = fy * coeff;
    vx -= fx * mcoeff;
    vy -= fy * mcoeff;

    double diff = distance(WATERTOWN) + radius - MAP_RADIUS;
    if (diff >= 0.0) {
        // Unit still outside of the map, reposition it
        moveTo(WATERTOWN, diff + EPSILON);
    }
}

struct TankerSpawn {
    int size;
    double angle;

    TankerSpawn(const int & size, const double & angle) {
        this->size = size;
        this->angle = angle;
    }
};

struct Player;

struct Looter : public Unit
{
    int skillCost;
    double skillRange;
    bool skillActive;

    Player * player;

    Point wantedThrustTarget = Point( 12000, 12000 );
    int wantedThrustPower = 0;

    // String message;
    // Action attempt;
    // SkillResult skillResult;

    Looter() {};

    Looter(int id, int type, Player * player,
           double x, double y, double vx, double vy);

    int getPlayerIndex() const;

    // virtual SkillEffect skillImpl(Point p) = 0;

    void setWantedThrust(const Point & target, int power);

    friend std::ostream &
    operator<<(std::ostream& os, const Looter& l)
    {
        os << "looter: "
           << l.id << " "
           << l.x << " " << l.y << " "
           << l.vx << " " << l.vy << " "
           << l.radius << " " << l.mass << " " << l.friction;
        return os;
    }
};

struct Reaper;
struct Destroyer;
struct Doof;

struct Player {
    int score = 0;
    int index;
    int rage = 0;
    Looter looters[LOOTER_COUNT];
    bool dead = false;
    // Queue<TankerSpawn> tankers;

    Player() {}

    Player(const int & index);

    void kill();

    Reaper * getReaper();

    Destroyer * getDestroyer();

    Doof * getDoof();
};

struct Reaper : public Looter {
    Reaper(Player * player, int id, double x, double y, double vx, double vy);

    // friend std::ostream &
    // operator<<(std::ostream& os, const Reaper& r)
    // {
    //     // Beautiful print
    //     os << "reaper: "
    //        << r.id << " " << r.x << " " << r.x << " " << r.vx << " " << r.vx;
    //     return os;
    // }
};

struct Destroyer : public Looter {
    Destroyer(Player * player, int id, double x, double y, double vx, double vy);

    // friend std::ostream &
    // operator<<(std::ostream& os, const Destroyer& d)
    // {
    //     // Beautiful print
    //     os << "destro: "
    //        << d.id << " " << d.x << " " << d.x << " " << d.vx << " " << d.vx;
    //     return os;
    // }
};

struct DoofSkillEffect;

struct Doof : public Looter {
    Doof(Player * player, int id, double x, double y, double vx, double vy);

    DoofSkillEffect skillImpl(Point p);

    int sing();

    // friend std::ostream &
    // operator<<(std::ostream& os, const Doof& d)
    // {
    //     // Beautiful print
    //     os << "doof  : "
    //        << d.id << " " << d.x << " " << d.x << " " << d.vx << " " << d.vx;
    //     return os;
    // }
};

struct Wreck : public Point {
    int id;
    double radius;
    int water;
    bool known;
    // Player player;

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
            if (isInRange(*p.getReaper(), radius))
            {
                p.score += 1;
                water -= 1;
            }
        }

        return water > 0;
    }
};

const static Wreck NULL_WRECK(12000, 12000, -1, -1);

struct Tanker : public Unit {
    int water;
    int size;
    Player * player;
    bool killed;

    Tanker(int id, int size, Player * player, double x, double y, double vx, double vy) :
        Unit (TYPE_TANKER, id, x, y, vx, vy) {
        this->player = player;
        this->size = size;

        water = TANKER_EMPTY_WATER;
        mass = TANKER_EMPTY_MASS + TANKER_MASS_BY_WATER * water;
        friction = TANKER_FRICTION;
        // radius = TANKER_RADIUS_BASE + TANKER_RADIUS_BY_SIZE * size;
        this->radius = size;

    }

    Wreck * die() {
        // Don't spawn a wreck if our center is outside of the map
        if (distance(WATERTOWN) >= MAP_RADIUS) {
            return NULL;
        }

        return new Wreck(round(x), round(y), water, radius);
    }

    bool isFull() {
        return water >= size;
    }

    void play() {
        if (isFull()) {
            // Try to leave the map
            thrust(WATERTOWN, -TANKER_THRUST);
        } else if (distance(WATERTOWN) > WATERTOWN_RADIUS) {
            // Try to reach watertown
            thrust(WATERTOWN, TANKER_THRUST);
        }
    }

    virtual
    Collision getCollision(bool verbose = false) {
        // Tankers can go outside of the map
        return NULL_COLLISION;
    }

    int getExtraInput() {
        return water;
    }

    int getExtraInput2() {
        return size;
    }

    inline void
    operator= (const Unit & u)
    {
        this->type = u.type;
        this->id   = u.id;
        this->x    = u.x;
        this->y    = u.y;
    }
};

// Tanker(int size, Player player) : Unit (TYPE_TANKER, -42, 0.0, 0.0) {
static Player NULL_PLAYER(-1);
const static Tanker NULL_TANKER(-42, -1, &NULL_PLAYER, 12000, 12000, 0, 0);

// Tanker Collision::dead
// Tanker * Collision::dead() {
Tanker * Collision::dead() {
    // if (a instanceof Destroyer && b instanceof Tanker && b.mass < REAPER_SKILL_MASS_BONUS) {
    if ((a->type == LOOTER_DESTROYER) && (b->type == TYPE_TANKER) &&
        (b->mass < REAPER_SKILL_MASS_BONUS)) {
        // return static_cast<Tanker *> (&b);
        // return dynamic_cast<Tanker *> (b);
        // return b;
        return static_cast<Tanker *> (b);
    }

    // if (b instanceof Destroyer && a instanceof Tanker && a.mass < REAPER_SKILL_MASS_BONUS) {
    if ((b->type == LOOTER_DESTROYER) && (a->type == TYPE_TANKER) &&
        (a->mass < REAPER_SKILL_MASS_BONUS)) {
        // return static_cast<Tanker *> (&a);
        // return dynamic_cast<Tanker *> (a);
        // return a;
        return static_cast<Tanker *> (a);
    }

    return NULL;
}


Reaper::Reaper(Player * player, int id, double x, double y, double vx, double vy) :
    Looter(id, LOOTER_REAPER, player, x, y, vx, vy) {
    // cerr << "REAPER id: " << id << endl;
    // cerr << "REAPER vx: " << this->vx << endl;
    // cerr << "REAPER vy: " << this->vy << endl;

    id = id;
    mass = REAPER_MASS;
    friction = REAPER_FRICTION;
    skillCost = REAPER_SKILL_COST;
    skillRange = REAPER_SKILL_RANGE;
    skillActive = REAPER_SKILL_ACTIVE;
    // cerr << "REAPER end" << endl;
}

Destroyer::Destroyer(Player * player, int id, double x, double y, double vx, double vy) :
    Looter(id, LOOTER_DESTROYER, player, x, y, vx, vy) {
    id = id;
    mass = DESTROYER_MASS;
    friction = DESTROYER_FRICTION;
    skillCost = DESTROYER_SKILL_COST;
    skillRange = DESTROYER_SKILL_RANGE;
    skillActive = DESTROYER_SKILL_ACTIVE;
}

// TO IMPLEMENT
// SkillEffect Destroyer::skillImpl(Point p) {
//     return new DestroyerSkillEffect(TYPE_DESTROYER_SKILL_EFFECT, p.x, p.y, DESTROYER_SKILL_RADIUS, DESTROYER_SKILL_DURATION,
//             DESTROYER_SKILL_ORDER, this);
// }

Doof::Doof(Player * player, int id, double x, double y, double vx, double vy) :
    Looter(id, LOOTER_DOOF, player, x, y, vx, vy) {
    id = id;
    mass = DOOF_MASS;
    friction = DOOF_FRICTION;
    skillCost = DOOF_SKILL_COST;
    skillRange = DOOF_SKILL_RANGE;
    skillActive = DOOF_SKILL_ACTIVE;
}

Player::Player(const int & index) {
    this->index = index;
}

void Player::kill() {
    dead = true;
}

Reaper * Player::getReaper() {
    return static_cast<Reaper* > (&looters[LOOTER_REAPER]);
}

Destroyer * Player::getDestroyer() {
    return static_cast<Destroyer* > (&looters[LOOTER_DESTROYER]);
}

Doof * Player::getDoof() {
    return static_cast<Doof* > (&looters[LOOTER_DOOF]);
}

Looter::Looter(int id, int type, Player * player,
               double x, double y, double vx, double vy) : Unit(type, id, x, y, vx, vy) {
    // cerr << "LOOTER id: " << id << endl;
    // cerr << "LOOTER vx: " << this->vx << endl;
    // cerr << "LOOTER vy: " << this->vy << endl;

    this->player = player;

    radius = LOOTER_RADIUS;
    // cerr << "LOOTER end" << endl;
}

int Looter::getPlayerIndex() const {
    return player->index;
}

void Looter::setWantedThrust(const Point & target, int power) {
    if (power < 0) {
        power = 0;
    }

    wantedThrustTarget = target;
    wantedThrustPower = min(power, MAX_THRUST);
}

std::vector<Unit *> units;
std::vector<Tanker> tankers;
std::vector<Tanker *> deadTankers;
std::vector<Wreck> wrecks;
std::vector<Player> players;
std::vector<SkillEffect *> skilleffects;

void print_units()
{
    for ( uint i = 0; i < units.size (); ++i)
    {
        if ( units[i] )
            cerr << *units[i] << endl;
    }
}


// unitFactory

// Get the next collision for the current round
// All units are tested
Collision getNextCollision(bool verbose = false) {
    Collision result = NULL_COLLISION;

    for (uint i = 0; i < units.size(); ++i) {
        // Unit unit = units.get(i);
        Unit * unit = units[i];

        if ( unit == NULL )
        {
            continue;
        }

        // Test collision with map border first
        // Collision collision;
        // if ( unit->id == 9 )
        //     collision = unit->getCollision(true);
        // else
        //     collision = unit->getCollision();
        Collision collision;
        if ( unit->type != TYPE_TANKER )
            collision = unit->getCollision();

        if (verbose) cerr << "COMPUTED Collision" << endl;
        if (verbose) cerr << "collision.t: " << collision.t << endl;
        if (verbose) if ( collision.a ) cerr << "collision.a: " << *collision.a << endl;
        if (verbose) if ( collision.b ) cerr << "collision.b: " << *collision.b << endl;
        if (verbose) if ( collision.b ) cerr << "collision.b->type: " << collision.b->type << endl;

        if (collision.t < result.t) {
            if (verbose) cerr << "0 UPDATE" << endl;
            if (verbose) cerr << "collision.t: " << collision.t << endl;
            if (verbose) if ( collision.a ) cerr << "collision.a: " << *collision.a << endl;
            if (verbose) if ( collision.b ) cerr << "collision.b: " << *collision.b << endl;
            if (verbose) if ( collision.b ) cerr << "collision.b->type: " << collision.b->type << endl;
            result = collision;
        }

        for (uint j = i + 1; j < units.size(); ++j) {
            // if (verbose) cerr << "IN LOOP j: " << j << endl;
            // if ( units[j] ) collision = unit->getCollision(*units[j], verbose);
            if ( units[j] ) collision = unit->getCollision(*units[j], false);

            if (collision.t < result.t) {
                if (verbose) cerr << "1 UPDATE" << endl;
                if (verbose) cerr << "collision.t: " << collision.t << endl;
                if (verbose) cerr << "collision.a: " << *collision.a << endl;
                if (verbose) cerr << "collision.b: " << *collision.b << endl;
                result = collision;
            }
        }
        // cerr << "0 result.t: " << result.t << endl;
    }

    if ( verbose ) cerr << "1 result.t: " << result.t << endl;
    if ( verbose ) if (result.a) cerr << "1 result.a: " << *result.a << endl;
    if ( verbose ) if (result.b) cerr << "1 result.b: " << *result.b << endl;
    return result;
}

// Play a collision
void playCollision(Collision collision) {
    // if (collision.b == &NULL_UNIT) {
    if (collision.b == NULL) {
        // Bounce with border
        // addToFrame(collision.a);
        collision.a->bounce();
    }
    else {
        Tanker * dead = collision.dead();
        // cerr << "IN Playcollision dead: " << dead << endl;
        // cerr << "IN Playcollision collision.b: " << collision.b << endl;

        // cerr << "IN Playcollision dead: " << dead << endl;

        if (dead != NULL) {
            // A destroyer kill a tanker
            // addDeadToFrame(dead);
            deadTankers.push_back(dead);

            // tankers.remove(*dead);
            for (uint i = 0; i < tankers.size() ; ++i)
            {
                Tanker & t = tankers[i];
                if ( dead == &t )
                {
                    t = NULL_TANKER;
                }
            }

            Tanker dead_save = *dead;
            // units.remove(*dead);
            for (uint i = 0; i < units.size() ; ++i)
            {
                Unit * u = units[i];
                if ( dead == u )
                // if ( *dead == *u )
                {
                    // cerr << "IN Playcollision find U: " << *u << endl;
                    // *u = NULL_UNIT;
                    // u = NULL_UNIT;
                    units[i] = NULL_UNIT;
                    break;
                }
            }
            // cerr << "IN Playcollision OUTSIDE LOOP" << endl;

            Wreck * wreck = dead->die();
            // Wreck * wreck = dead_save.die();

            // If a tanker is too far away, there's no wreck
            if (wreck != NULL) {
                wrecks.push_back(*wreck);
                // addToFrame(wreck);
            }
            // cerr << "IN Playcollision END" << endl;
        } else {
            // Bounce between two units
            // addToFrame(collision.a);
            // addToFrame(collision.b);
            collision.a->bounce(*collision.b);
        }
    }
}

struct SkillEffect : public Point {
    int id;
    int type;
    double radius;
    int duration;
    int order;
    bool known;
    Looter looter;

    SkillEffect(int type, double x, double y, double radius,
                int duration, int order);

    void apply(std::vector<Unit *> & units);

    virtual void applyImpl(Unit & unit) = 0;

};

SkillEffect::SkillEffect(int type, double x, double y, double radius,
                         int duration, int order) : Point(x, y) {
    // super(x, y);

    // id = GLOBAL_ID++;

    this->type = type;
    this->radius = radius;
    this->duration = duration;
    // this->looter = looter;
    this->order = order;
}

void SkillEffect::apply(std::vector<Unit *> & units) {
    // applyImpl(units.stream().filter(u -> isInRange(u, radius + u.radius)).collect(Collectors.toList()));
    duration -= 1;
    for (uint i = 0; i < units.size(); ++i)
    {
        Unit & u = *units[i];
        if ( isInRange(u, radius + u.radius) )
        {
            applyImpl (u);
        }
    }
        
}


struct ReaperSkillEffect : public SkillEffect {
    ReaperSkillEffect(int type, double x, double y, double radius, int duration, int order) : SkillEffect(type, x, y, radius, duration, order) {
        // super(type, x, y, radius, duration, order, reaper);
    }

    // void applyImpl(List<Unit> units) {
    //     // Increase mass
    //     units.forEach(u -> u.mass += REAPER_SKILL_MASS_BONUS);
    // }
    void applyImpl(Unit & u) {
        // Increase mass
        u.mass += REAPER_SKILL_MASS_BONUS;
    }
};

struct DestroyerSkillEffect : public SkillEffect {

    DestroyerSkillEffect(int type, double x, double y, double radius, int duration, int order) : SkillEffect(type, x, y, radius, duration, order) {
        // super(type, x, y, radius, duration, order, destroyer);
    }

    // void applyImpl(List<Unit> units) {
    //     // Push units
    //     units.forEach(u -> u.thrust(this, -DESTROYER_NITRO_GRENADE_POWER));
    // }
    void applyImpl(Unit & u) {
        // Push units
        u.thrust(*this, -DESTROYER_NITRO_GRENADE_POWER);
    }
};

struct DoofSkillEffect : public SkillEffect {

    DoofSkillEffect(int type, double x, double y, double radius, int duration, int order) : SkillEffect(type, x, y, radius, duration, order){
        // super(type, x, y, radius, duration, order, doof);
    }

    // void applyImpl(List<Unit> units) {
    //     // Nothing to do now
    // }
    void applyImpl(Unit & u) {
        // Nothing to do now
    }
};


DoofSkillEffect Doof::skillImpl(Point p) {
    return DoofSkillEffect(TYPE_DOOF_SKILL_EFFECT, p.x, p.y, DOOF_SKILL_RADIUS, DOOF_SKILL_DURATION, DOOF_SKILL_ORDER);
}

// With flame effects! Yeah!
int Doof::sing() {
    return (int) floor(speed() * DOOF_RAGE_COEF);
}

bool Unit::isInDoofSkill(std::vector<SkillEffect> & skillEffects) {
    // return skillEffects.stream().anyMatch(s -> s instanceof DoofSkillEffect && isInRange(s, s.radius + radius));
    
    bool res = false;
    for (uint i = 0; skillEffects.size() ; ++i)
    {
        SkillEffect & s = skillEffects[i];
        // if ( std::is_base_of<DoofSkillEffect, s> && isInRange(s, s.radius + radius) )
        if (( s.type == TYPE_DOOF_SKILL_EFFECT) && isInRange(s, s.radius + radius) )
        {
            res = true;
            break;
        }
    }
    
    return res;
}

void Unit::adjust(std::vector<SkillEffect> & skillEffects) {
    x = round(x);
    y = round(y);

    if (isInDoofSkill(skillEffects)) {
        // No friction if we are in a doof skill effect
        vx = round(vx);
        vy = round(vy);
    } else {
        vx = round(vx * (1.0 - friction));
        vy = round(vy * (1.0 - friction));
    }
}

Unit * Unit::make_unit(int id, int type, int player_id, int size, double x, double y, double vx, double vy)
// Point * Point::make_unit(int type, int player_id, int size, double x, double y, int water)
{
    Player * player = &players[player_id];
    Reaper * r;
    switch (type){
    case LOOTER_REAPER:
        r = new Reaper(player, id, x, y, vx, vy);
        // cerr << "r->vx: " << r->vx << endl;
        // cerr << "r->vy: " << r->vy << endl;
        return r;
        break;
    case LOOTER_DESTROYER:
        return new Destroyer(player, id, x, y, vx, vy);
        break;
    case LOOTER_DOOF:
        return new Doof(player, id, x, y, vx, vy);
        break;
    case TYPE_TANKER:
        return new Tanker(id, size, player, x, y, vx, vy);
        break;
    // case TYPE_WRECK:
    //     return new Wreck(x, y, water, size);
    //     break;
    }

    return NULL;
}


std::vector<SkillEffect> skillEffects;
const static int NB_PLAYER = 3;
struct Simulation
{
    Simulation () {}

    static void adjust() {
      for (uint i = 0; i < units.size (); ++i)
      {
        Unit * u = units[i];
        if (u) u->adjust (skillEffects);
      }
    }

    static void
    // updateGame(int round)
    updateGame()
    {
        // cerr << "PASS 0" << endl;
        // Apply skill effects
        for (uint i = 0; i < skillEffects.size () ; ++i)
        {
            SkillEffect & e = skillEffects[i];
            e.apply(units);
        }

        // // Apply thrust for tankers
        // for (uint i = 0; i < skillEffects.size () ; ++i)
        // {
        //     Tanker & t = tankers[i];
        //     t.play ();
        // }
        for (uint i = 0; i < units.size () ; ++i )
        {
            Unit * u = units[i];
            if ( u->type == TYPE_TANKER )
            {
                Tanker * t = static_cast<Tanker *> ( u );
                t->play ();
            }
        }

        // Apply wanted thrust for looters
        // for (int idx_player = 0; idx_player < NB_PLAYER ; ++idx_player) {
        //     Player p = players[idx_player];
        //     for (int idx_looter = 0; idx_looter < LOOTER_COUNT; ++idx_looter) {
        //         Looter l = p.looters[idx_looter];
        //         if (l.wantedThrustTarget != NULL_POINT) {
        //             l.thrust(l.wantedThrustTarget, l.wantedThrustPower);
        //         }
        //     }
        // }

        for (uint i = 0; i < units.size () ; ++i )
        {
            Unit * u = units[i];
            if ( ( u->type == LOOTER_REAPER ) ||
                 ( u->type == LOOTER_DESTROYER ) ||
                 ( u->type == LOOTER_DOOF ) )
            {
                Looter * l = static_cast<Looter *> ( u );
                if (l->wantedThrustTarget != NULL_POINT) {
                    l->thrust (l->wantedThrustTarget, l->wantedThrustPower);
                }
            }
        }

        // cerr << "PASS 1" << endl;
        double t = 0.0;

        // Play the round. Stop at each collisions and play it. Reapeat until t > 1.0

        // cerr << "OUTER BEGIN" << endl;
        // Collision collision = getNextCollision(true);
        Collision collision = getNextCollision();
        // cerr << "OUTER collision.t: " << collision.t << endl;
        // if (collision.a) cerr << "OUTER collision.a: " << *collision.a << endl;
        // if (collision.b) cerr << "OUTER collision.b: " << *collision.b << endl;

        // print_units();
        // cerr << endl;

        // int i_dbg = 0;
        while (collision.t + t <= 1.0) {
            // cerr << "collision.t + t: " << collision.t + t << endl;

            double delta = collision.t;
            for (uint i = 0 ; i < units.size () ; ++i) {
                if ( units[i] ) units[i]->move (delta);
            }
            t += collision.t;
            // cerr << "IN COLLISION t: " << t << endl;

            // cerr << "BEFORE Playcollision collision.t: " << collision.t << endl;
            // if (collision.a) cerr << "collision.a: " << *collision.a << endl;
            // if (collision.b) cerr << "collision.b: " << *collision.b << endl;

            playCollision(collision);
            // cerr << "AFTER Playcollision" << endl;
            // if (collision.a) cerr << "collision.a: " << *collision.a << endl;
            // if (collision.b) cerr << "collision.b: " << *collision.b << endl;

            // print_units();
            // cerr << "1 units[9]: " << *units[9] << endl;

            // cerr << "i_dbg: " << i_dbg << endl;
            // if ( i_dbg == 3 )
            //     collision = getNextCollision(true);
            // else
            //     collision = getNextCollision();

            // collision = getNextCollision(true);
            collision = getNextCollision();

            // if (i_dbg == 3)
            // {
            //     cerr << "NEW collision.t: " << collision.t << endl;
            //     cerr << "collision.t + t: " << collision.t + t << endl;
            //     if (collision.a) cerr << "collision.a: " << *collision.a << endl;
            //     if (collision.b) cerr << "collision.b: " << *collision.b << endl;
            // }

            // print_units();
            // cerr << endl;
            // ++i_dbg;
        }

        // cerr << "PASS 2" << endl;
        // No more collision. Move units until the end of the round
        double delta = 1.0 - t;
        for (uint i = 0 ; i < units.size () ; ++i) {
            if ( units[i] ) units[i]->move (delta);
        }

        // List<Tanker> tankersToRemove = new ArrayList<>();
        // std::vector<Tanker> tankersToRemove;
        std::vector<Tanker *> tankersToRemove;

        // for (uint i = 0; i < tankers.size(); ++i)
        // {
        //     Tanker & tanker = tankers[i];
        //     double distance = tanker.distance(WATERTOWN);
        //     bool full = tanker.isFull();

        //     if (distance <= WATERTOWN_RADIUS && !full) {
        //         // A non full tanker in watertown collect some water
        //         tanker.water += 1;
        //         tanker.mass += TANKER_MASS_BY_WATER;
        //     }
        //     else if (distance >= TANKER_SPAWN_RADIUS + tanker.radius && full) {
        //         // Remove too far away and not full tankers from the game
        //         tankersToRemove.push_back(tanker);
        //     }
        // }

        // cerr << "UPDATEGAME PASS  0" << endl;
        
        for (uint i = 0; i < units.size () ; ++i )
        {
            Unit * u = units[i];
            if (u && ( u->type == TYPE_TANKER ))
            {
                Tanker * t = static_cast<Tanker *> ( u );
                double distance = t->distance(WATERTOWN);
                bool full = t->isFull();

                if (distance <= WATERTOWN_RADIUS && !full) {
                    // A non full tanker in watertown collect some water
                    t->water += 1;
                    t->mass += TANKER_MASS_BY_WATER;
                }
                else if (distance >= TANKER_SPAWN_RADIUS + t->radius && full) {
                    // Remove too far away and not full tankers from the game
                    tankersToRemove.push_back(t);
                }
            }
        }

        
        // TO IMPLEMENT
        // newFrame(1.0);
        // snapshot();

        // if (!tankersToRemove.empty ()) {
        //     tankersToRemove.forEach(tanker -> addDeadToFrame(tanker));
        // }

        // cerr << "PASS 3" << endl;
        // units.removeAll(tankersToRemove);
        for (uint i = 0; i < tankersToRemove.size(); ++i)
        {
            // Tanker & tanker = tankersToRemove[i];
            Tanker * tanker = tankersToRemove[i];
            for (uint j = 0; j < units.size(); ++j)
            {
                // Unit & unit = *units[j];
                Unit * unit = units[j];
                if ( unit == NULL )
                    continue;
                // if (unit == tanker)
                if (unit == tanker)
                {
                    unit = NULL_UNIT;
                    break;
                }
            }
        }
        
        // cerr << "UPDATEGAME PASS  1" << endl;
        // // tankers.removeAll(tankersToRemove);
        // for (uint i = 0; i < tankersToRemove.size(); ++i)
        // {
        //     Tanker & tanker = tankersToRemove[i];
        //     for (uint j = 0; j < tankers.size(); ++j)
        //     {
        //         Tanker & tanker_origin = tankers[j];
        //         if (tanker_origin == tanker)
        //         {
        //             tanker_origin = NULL_UNIT;
        //             break;
        //         }
        //     }
        // }
        // tankers.removeAll(tankersToRemove);
        // for (uint i = 0; i < tankersToRemove.size(); ++i)
        // {
        //     Tanker * tanker = tankersToRemove[i];
        //     for (uint j = 0; j < tankers.size(); ++j)
        //     {
        //         Tanker & tanker_origin = tankers[j];
        //         if (tanker_origin == tanker)
        //         {
        //             tanker_origin = NULL_UNIT;
        //             break;
        //         }
        //     }
        // }
        
        // deadTankers.addAll(tankersToRemove);
        // for (uint i = 0; i < tankersToRemove.size(); ++i)
        // {
        //     Tanker & tanker = tankersToRemove[i];
        //     for (uint j = 0; j < deadTankers.size(); ++j)
        //     {
        //         Tanker & tanker_die = deadTankers[j];
        //         if (tanker_die == tanker)
        //         {
        //             tanker_die = NULL_UNIT;
        //             break;
        //         }
        //     }
        // }
        for (uint i = 0; i < tankersToRemove.size(); ++i)
        {
            Tanker * tanker = tankersToRemove[i];
            for (uint j = 0; j < deadTankers.size(); ++j)
            {
                Tanker * tanker_die = deadTankers[j];
                if (tanker_die == tanker)
                {
                    // tanker_die = NULL_UNIT;
                    tanker_die = NULL;
                    break;
                }
            }
        }

        // cerr << "UPDATEGAME PASS  2" << endl;
        // // Spawn new tankers for each dead tanker during the round
        // deadTankers.forEach(tanker -> spawnTanker(tanker.player));
        // deadTankers.clear();

        // Set<Wreck> deadWrecks = new HashSet<>();
        std::vector<Wreck> deadWrecks;

        // Water collection for reapers
        // wrecks = wrecks.stream().filter(w -> {
        //     bool alive = w.harvest(players, skillEffects);

        //     if (!alive) {
        //         addDeadToFrame(w);
        //         deadWrecks.add(w);
        //     }

        //     return alive;
        // }).collect(Collectors.toList());
        for (uint i = 0; i < wrecks.size () ; i++)
        {
          Wreck & w = wrecks[i];
          // bool alive = w.harvest(players, skillEffects);
          bool alive = w.harvest(players);
          
          if (!alive)
          {
            // addDeadToFrame(w);
            deadWrecks.push_back(w);
            w = NULL_WRECK;
          }
        }

        // cerr << "UPDATEGAME PASS  3" << endl;
        // cerr << "PASS 4" << endl;

        // if (SPAWN_WRECK) {
        //     deadWrecks.forEach(w -> spawnTanker(w.player));
        // }

        // Round values and apply friction
        adjust();

        // cerr << "UPDATEGAME PASS  3" << endl;

        // Generate rage
        // players.forEach(p -> p.rage = Math.min(MAX_RAGE, p.rage + p.getDoof().sing()));
        for( uint i = 0 ; i < NB_PLAYER ; ++i)
        {
            Player & p = players[i];
            p.rage = std::min( MAX_RAGE, p.rage + p.getDoof()->sing());
        }

        // // Restore masses
        // units.forEach(u -> {
        //     while (u.mass >= REAPER_SKILL_MASS_BONUS) {
        //         u.mass -= REAPER_SKILL_MASS_BONUS;
        //     }
        // });
        for (uint i = 0; i < units.size (); ++i)
        {
            Unit * u = units[i];
            if (u == NULL)
            {
                continue;
            }
            while (REAPER_SKILL_MASS_BONUS <= u->mass)
            {
                u->mass -= REAPER_SKILL_MASS_BONUS;
            }
        }

        // // Remove dead skill effects
        // Set<SkillEffect> effectsToRemove = new HashSet<>();
        // for (SkillEffect effect : skillEffects) {
        //     if (effect.duration <= 0) {
        //         addDeadToFrame(effect);
        //         effectsToRemove.add(effect);
        //     }
        // }
        // skillEffects.removeAll(effectsToRemove);
        // std::vector<SkillEffect> effectsToRemove;
        for (uint i = 0; i < skillEffects.size () ; ++i)
        {
            SkillEffect & e = skillEffects[i];
            if (e.duration <= 0)
            {
                // effectsToRemove.push_back(e);
                e.x = 12000;
                e.y = 12000;
                e.radius = 0.;
                e.duration = -1;
            }
        }

    }

};

std::vector<Unit *> units_previous;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    players.push_back( Player(0) );
    players.push_back( Player(1) );
    players.push_back( Player(2) );

    // game loop
    while (1) {
        units.clear ();
        skilleffects.clear ();
        wrecks.clear ();

        int myScore;
        cin >> myScore; cin.ignore();
        int enemyScore1;
        cin >> enemyScore1; cin.ignore();
        int enemyScore2;
        cin >> enemyScore2; cin.ignore();
        int myRage;
        cin >> myRage; cin.ignore();
        int enemyRage1;
        cin >> enemyRage1; cin.ignore();
        int enemyRage2;
        cin >> enemyRage2; cin.ignore();

        players[0].score = myScore;
        players[0].rage = myRage;
        players[1].score = enemyScore1;
        players[1].rage = enemyRage1;
        players[2].score = enemyScore2;
        players[2].rage = enemyRage2;

        int unitCount;
        cin >> unitCount; cin.ignore();
        for (int i = 0; i < unitCount; i++) {
            int unitId;
            int unitType;
            int player;
            float mass;
            int radius;
            int x;
            int y;
            int vx;
            int vy;
            int extra;
            int extra2;
            cin >> unitId >> unitType >> player >> mass >> radius >> x >> y >> vx >> vy >> extra >> extra2; cin.ignore();

            // cerr << unitId << " " << unitType <<  " " << player << " " << mass << " " << radius << " " << x << " " << y << " " << vx << " " << vy << " " << extra << " " << extra2 << endl;

            if ( (unitType == LOOTER_REAPER) ||
                 (unitType == LOOTER_DESTROYER) ||
                 (unitType == LOOTER_DOOF) ||
                 (unitType == TYPE_TANKER) )
            {
                units.push_back(Unit::make_unit(unitId, unitType, player, radius, x, y, vx, vy));
            }
            else if ( unitType == TYPE_WRECK )
            {
                wrecks.push_back (Wreck(x, y, extra, radius));
            }
            else if ( unitType == TYPE_REAPER_SKILL_EFFECT )
            {
                skilleffects.push_back(new ReaperSkillEffect(unitType, x, y,
                                                             REAPER_SKILL_RADIUS, extra,
                                                             REAPER_SKILL_ORDER));
            }
            else if ( unitType == TYPE_DOOF_SKILL_EFFECT )
            {
                skilleffects.push_back(new DoofSkillEffect(unitType, x, y,
                                                           REAPER_SKILL_RADIUS, extra,
                                                           REAPER_SKILL_ORDER));
            }
        }

        cerr << "before simulation" << endl;
        print_units();
        cerr << endl;

        int j = 0;
        for ( uint i = 0; i < units_previous.size () ; ++i )
        {
            if ( units_previous[i] )
            {
                if ( *units_previous[i] != *units[j] )
                {
                    cerr << "DIFFERENCES: " << endl;
                    cerr << "previous(" << i << "): " << *units_previous[i] << endl;
                    cerr << "current (" << j << "): " << *units[j]  << endl;
                }
                ++j;
            }
        }

        Point wantedTarget = Point( units[1]->x, units[1]->y);;
        int wantedPower = 300;

        static_cast<Looter *> (units[0])->wantedThrustTarget = wantedTarget;
        static_cast<Looter *> (units[0])->wantedThrustPower = wantedPower;
        
        using time_point = chrono::time_point< chrono::system_clock > ;
        using duration   = chrono::duration< float > ;

        time_point start ;
        duration d ;

        start = now() ;
        Simulation::updateGame();
        d = now() - start ;
        cerr << "Total time: " << d.count( ) * 1000 << " ms" << endl ;

        units_previous = units;

        // cerr << "after simulation" << endl;
        // print_units();

        // cout << "WAIT" << endl;
        cout << wantedTarget.x << " " << wantedTarget.y << " " << wantedPower << endl;
        cout << "WAIT" << endl;
        cout << "WAIT" << endl;
    }
}
