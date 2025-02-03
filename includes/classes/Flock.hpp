#ifndef FLOCK_HPP
# define FLOCK_HPP

// INCLUDES
# include "./Boid.hpp"

// CLASS DEFINITIONS
class Flock
{
    public:
		t_globaloptions options;
        Boid boids[NB_BOIDS];
        Flock(void);
        ~Flock(void);
		void average(void);
		void separate(void);
        void align(void);
		void cohese(void);
        void update(void);
        void mirror(void);
        void draw(t_globaloptions options);
};

#endif
