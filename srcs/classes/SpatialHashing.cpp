#include "../../includes/classes/SpatialHashing.hpp"

// CONSTRUCTOR
SpatialHashing::SpatialHashing(void)
{
	const float div = sqrtf(HASH_SIZE);
	const float grid_height = HEIGHT / div;
	const float grid_width = WIDTH / div;
	float xi = 0;
	float yi = 0;

	for (size_t i = 0; i < HASH_SIZE; i++)
	{
		this->table[i].rect = {xi, yi, grid_width, grid_height};
		this->table[i].center = {xi + grid_width / 2, yi + grid_height / 2};
		this->table[i].boids = NULL;
		if (xi + grid_width >= WIDTH)
		{
			xi = 0;
			yi += grid_height;
		}
		else
			xi += grid_width;
	}
}

// DESTRUCTOR
SpatialHashing::~SpatialHashing(void)
{
	t_boid_list *tmp = NULL;
	t_boid_list *next = NULL;

	for (size_t i = 0; i < HASH_SIZE; i++)
	{
		tmp = this->table[i].boids;
		while (tmp != NULL)
		{
			next = tmp->next;
			delete tmp;
			tmp = next;
		}
	}
}

// MEMBER FUNCTIONS
int		SpatialHashing::hash(Vector2 center)
{
	const int hash_grid = sqrt(HASH_SIZE);
	const int x = static_cast<int>(center.x) / (WIDTH / hash_grid);
	const int y = static_cast<int>(center.y) / (HEIGHT / hash_grid);

	return (x + y * hash_grid);
}

void	SpatialHashing::clear(void)
{
	t_boid_list *tmp = NULL;
	t_boid_list *next = NULL;

	for (size_t i = 0; i < HASH_SIZE; i++)
	{
		tmp = this->table[i].boids;
		while (tmp != NULL)
		{
			next = tmp->next;
			delete tmp;
			tmp = next;
		}
		this->table[i].boids = NULL;
	}
}

void	SpatialHashing::insert(Boid *boid)
{
	const int index = this->hash(boid->properties.pos);

	t_boid_list *new_boid = new t_boid_list;
	new_boid->boid = boid;
	new_boid->next = this->table[index].boids;
	this->table[index].boids = new_boid;
}

void	SpatialHashing::draw_rect(Rectangle rect, Color color)
{
	DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, color);
}

void	SpatialHashing::draw(void)
{
	for (size_t i = 0; i < HASH_SIZE; i++)
		if (this->table[i].boids == nullptr)
			this->draw_rect(this->table[i].rect, GRAY);
	for (size_t i = 0; i < HASH_SIZE; i++)
		if (this->table[i].boids != nullptr)
			this->draw_rect(this->table[i].rect, GREEN);
}
