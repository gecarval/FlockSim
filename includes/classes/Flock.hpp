#ifndef FLOCK_HPP
# define FLOCK_HPP

// INCLUDES
# include "./Boid.hpp"

// CLASS DEFINITIONS
class Flock
{
    public:
        Boid boids[NB_BOIDS];
        Flock();
        ~Flock();
        void align();
        void update();
        void draw();
        void mirror();
};

#endif
