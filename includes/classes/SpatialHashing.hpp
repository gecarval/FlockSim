#ifndef SPATIALHASHING_HPP
# define SPATIALHASHING_HPP

# include "Boid.hpp"

// MACROS
const int HASH_QUAD_SIZE = 100;
const int HASH_CALC = (((CANVAS_WIDTH + CANVAS_HEIGHT) / 2) / HASH_QUAD_SIZE);
const int HASH_LEN = HASH_CALC * HASH_CALC;
// STRUCTURES

typedef struct s_food
{
	Vector2	pos;
	float	radius;
	float	energy;
	struct s_food	*next;
}				t_food;

typedef struct s_food_list
{
	t_food			*food;
	struct s_food_list	*next;
}				t_food_list;

typedef struct t_boid_list
{
	Boid				*boid;
	struct t_boid_list	*next;
}				t_boid_list;

typedef struct s_hash_table
{
	Rectangle	rect;
	Vector2		center;
	t_boid_list *boids;
	t_food_list	*food;
}				t_hash_table;

// CLASS DEFINITIONS
class SpatialHashing
{
	public:
		t_hash_table	table[HASH_LEN];
		SpatialHashing(void);
		~SpatialHashing(void);
		void	clear(void);
		int		hash(Vector2 center);
		void	insert(Boid *boid);
		void	insert(t_food *food);
		void	draw(Camera2D camera);
        void    drawhashmaptexture(RenderTexture2D texture);
        void    renderhashmaptexture(RenderTexture2D texture);

	private:
		void	draw_rect(Rectangle rect, Color color);
};

#endif
