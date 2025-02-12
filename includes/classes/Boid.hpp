#ifndef BOID_HPP
# define BOID_HPP

// MACROS
# define WIDTH 3000
# define HEIGHT 2000
# define NB_BOIDS 1000
# define BOID_SIZE 8

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

typedef struct s_boid
{
	Vector2		pos;
	Color		color;
	float		perception;
	float		separation_ratio;
	float		min_speed;
	float		max_speed;
	float		max_steer;
	float		obstacle_avoidance;
	float		max_alignment;
	float		max_cohesion;
	float		max_separation;
}				t_boid;

// CLASS DEFINITIONS
class Boid
{
	private:
		float	frame_time_counter;

	protected:
		Vector2	acc;
		float	rotation;
		int		sides;

	public:
		float	radius;
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
		void	update(float gamespeed);
		void	mirror(void);
		void	avoidborder(float gamespeed);
		void	draw_boid(void);
		void	draw_perception(void);
		void	draw_velocity(void);
		void	draw_align(void);
		void	draw_cohese(void);
		void	draw_avoid(void);
};

#endif
