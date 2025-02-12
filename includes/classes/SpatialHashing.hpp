#ifndef SPATIALHASHING_HPP
# define SPATIALHASHING_HPP

# include "Boid.hpp"
# include <string.h>

// MACROS
# define HASH_SIZE 225000

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
		t_hash_table	table[HASH_SIZE];
		SpatialHashing(void);
		~SpatialHashing(void);
		void	clear(void);
		int		hash(Vector2 center);
		void	insert(Boid *boid);
		void	draw(void);

	private:
		void	draw_rect(Rectangle rect, Color color);
};

#endif
