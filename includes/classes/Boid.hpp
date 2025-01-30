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

// DATA STRUCTURES
typedef struct s_check_box
{
	bool	draw;
	bool	draw_perception;
	bool	draw_velocity;
}				t_check_box;

// CLASS DEFINITIONS
class Boid
{
    private:
        float	frame_time_counter;
        float	radius;

    protected:
        Vector2	vel;
        Vector2	acc;
        float	rotation;
        int		sides;

    public:
        Vector2		pos;
        Color	    color;
        t_check_box	check;
        float		perception;
        float		max_speed;
        float		min_speed;
        Boid();
        Boid(Vector2 pos, float perception, float min_speed, float max_speed, Color col);
        ~Boid();
        void	draw();
        void	align(Boid *flock);
        void	update();
        void	mirror();
};

#endif
