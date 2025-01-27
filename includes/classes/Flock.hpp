#ifndef FLOCK_HPP
# define FLOCK_HPP

// INCLUDES
# include "./Boid.hpp"

// CLASS DEFINITIONS
class Flock {
    private:
        Boid boids[NB_BOIDS];

    public:
        Flock();
        ~Flock();
        void align();
        void update();
        void draw();
        void mirror();
};

#endif
