#ifndef BOID_HPP
# define BOID_HPP

// MACROS
# define WIDTH 1600
# define HEIGHT 900
# define NB_BOIDS 100
# define BOID_SIZE 3
# define STEERING 0.03

// INCLUDES
# include "raylib.h"
# include "raymath.h"

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
        void align(Boid *flock);
        void update();
        void mirror();
};

#endif
