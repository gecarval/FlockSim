#ifndef BOID_HPP
# define BOID_HPP

# include "../game.hpp"

// CLASS DEFINITIONS
class Boid {
    protected:
        Color   color;
        int   radius;

    public:
        Vector2 pos;
        Vector2 vel;
        Vector2 acc;
        Boid();
        Boid(Vector2 pos);
        ~Boid();
        void draw();
        void update();
        void mirror();
};

#endif
