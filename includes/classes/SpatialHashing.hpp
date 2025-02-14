#ifndef SPATIALHASHING_HPP
# define SPATIALHASHING_HPP

# include "Boid.hpp"
# include <string.h>

// MACROS
# define HASH_QUAD_SIZE 100
# define HASH_CALC (((WIDTH + HEIGHT) / 2) / HASH_QUAD_SIZE)
# define HASH_LEN HASH_CALC * HASH_CALC
// STRUCTURES

typedef struct t_boid_list
{
	Boid		*boid;
	struct t_boid_list	*next;
}				t_boid_list;

typedef struct s_hash_table
{
	Rectangle	rect;
	Vector2		center;
	t_boid_list *boids;
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
		void	draw(Camera2D camera);
        void    drawhashmaptexture(RenderTexture2D texture);
        void    renderhashmaptexture(RenderTexture2D texture);

	private:
		void	draw_rect(Rectangle rect, Color color);
};

#endif
