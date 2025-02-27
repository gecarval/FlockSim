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
	bool	draw_food;
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
	int		boids_alive;
	int		food_amount;
}				t_globaloptions;

// CLASS DEFINITIONS
class Flock
{
    public:
		t_check_box		check;
		t_globaloptions	options;
        Boid boids[NB_BOIDS];
		t_food *food;
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
		void updateflock(void);
		void lifeupdate(void);
		void remove_food(t_food *food);
		void generate_food(void);
		bool generate_one_food(Circle circle, bool oncollision);
		bool generate_one_food(Rectangle rect, bool oncollision);
		void generate_food_overtime(float gamespeed, Rectangle rect, bool oncollision);
		void generate_food_overtime(float gamespeed, Circle circle, bool oncollision);
        void draw(Camera2D camera, RenderTexture2D texture);
};

#endif
