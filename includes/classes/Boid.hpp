#ifndef BOID_HPP
# define BOID_HPP

// MACROS
# define WIDTH 1600
# define HEIGHT 900
# define NB_BOIDS 100
# define BOID_SIZE 8

// INCLUDES
# include "raylib.h"
# include "raymath.h"
# include <iostream>

// STRUCTURES
typedef struct s_globaloptions
{
	bool	show_fps;
	bool	mirror;
	bool	separate;
	bool	align;
	bool	cohese;
}				t_globaloptions;


typedef struct s_check_box
{
	bool	draw;
	bool	draw_perception;
	bool	draw_velocity;
}				t_check_box;

typedef struct s_average
{
    Vector2		vel;
    Vector2		pos;
    Vector2		sep;
}				t_average;

typedef struct s_boid
{
    Vector2		pos;
    Color		color;
    t_check_box	check;
    float		perception;
    float		min_speed;
    float		max_speed;
    float		max_steer;
    float   avoid_force;
    float		max_alignment;
    float		max_cohesion;
    float		max_separation;
}				t_boid;

// CLASS DEFINITIONS
class Boid
{
    private:
        float	frame_time_counter;
        float	radius;

    protected:
        Vector2	acc;
        float	rotation;
        int		sides;

    public:
        Vector2 vel;
        t_boid  properties;
		t_average average;
        Boid(t_boid properties);
        Boid(void);
        ~Boid(void);
        void	getaverage(Boid *flock);
		void	separate(void);
        void	align(void);
		void	cohese(void);
        void	update(void);
        void	mirror(void);
        void  avoidborder(void);
        void	draw(t_globaloptions options);
};

#endif
