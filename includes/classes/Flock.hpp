#ifndef FLOCK_HPP
# define FLOCK_HPP

// INCLUDES
# include "./SpatialHashing.hpp"

// STRUCTURES

typedef struct Circle
{
	Vector2 pos;
	float radius;
}				Circle;

typedef struct s_check_box
{
	bool	draw;
	bool	draw_perception;
	bool	draw_velocity;
	bool	draw_hash;
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
		SpatialHashing hash;
        Flock(void);
        ~Flock(void);
		void gethash(void);
		void average(void);
		void getaverage(void);
		void separate(void);
        void align(void);
		void cohese(void);
        void update(void);
        void mirror(void);
        void draw(void);
};

#endif
