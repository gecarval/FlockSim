#ifndef BOID_HPP
# define BOID_HPP

// MACROS
# define WIDTH 2000
# define HEIGHT 2000
# define NB_BOIDS 1000
# define BOID_SIZE 3

// INCLUDES
# include "raylib.h"
# include "raymath.h"
# include <iostream>

// STRUCTURES
typedef struct s_average
{
	Vector2		vel;
	Vector2		pos;
	Vector2		sep;
}				t_average;

typedef struct s_lifestats
{
	float		health;
	float		energy;
	float		age;
	int			generation;
	int			children;
	float		food;
}				t_lifestats;

typedef struct s_boid
{
	t_lifestats	life;
	int			id;
	Vector2		pos;
	Color		color;
	float		perception;
	float		min_speed;
	float		max_speed;
	float		max_steer;
	float		max_alignment;
	float		max_cohesion;
	float		max_separation;
	float		separation_ratio;
	float		obstacle_avoidance;
}				t_boid;

// CLASS DEFINITIONS
class Boid
{
	protected:
		Vector2	acc;
		float	rotation;

	public:
		float		radius;
		Vector2		vel;
		t_boid		stats;
		t_average	average;

	public:
		~Boid(void);
		Boid(void);
		Boid(t_boid stats);
		void	getaverage(Boid *flock);
		void	separate(void);
		void	align(void);
		void	cohese(void);
		void	update(float gamespeed);
		void	lifestatsupdate(void);
		void	mirror(void);
		void	avoidborder(void);
		void	draw_boid(void);
		void	draw_perception(void);
		void	draw_velocity(void);
		void	draw_align(void);
		void	draw_cohese(void);
		void	draw_avoid(void);
};

#endif
