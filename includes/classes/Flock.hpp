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
	bool	align;
	bool	separate;
	bool	cohese;
	bool	mirror;
	bool	avoidborder;
	int		alignAlgorithm;
	float	gamespeed;
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
		void hashaverage(void);
		void separate(void);
        void align(void);
		void cohese(void);
        void update(void);
        void mirror(void);
		void avoidborder(void);
        void draw(void);
};

#endif
