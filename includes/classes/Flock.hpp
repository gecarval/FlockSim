#ifndef FLOCK_HPP
# define FLOCK_HPP

// INCLUDES
# include "./Boid.hpp"
# include "./SpatialHashing.hpp"

// STRUCTURES
typedef struct s_check_box
{
	bool	draw;
	bool	draw_perception;
	bool	draw_velocity;
}				t_check_box;

typedef struct s_globaloptions
{
	bool	show_fps;
	bool	mirror;
	bool	separate;
	bool	align;
	bool	cohese;
}				t_globaloptions;

// CLASS DEFINITIONS
class Flock
{
    public:
		t_check_box		check;
		t_globaloptions	options;
        Boid boids[NB_BOIDS];
        Flock(void);
        ~Flock(void);
		void average(void);
		void separate(void);
        void align(void);
		void cohese(void);
        void update(void);
        void mirror(void);
        void draw(void);
};

#endif
