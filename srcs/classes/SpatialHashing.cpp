#include "../../includes/classes/SpatialHashing.hpp"

// CONSTRUCTOR
SpatialHashing::SpatialHashing(void)
{
	const float div = sqrt(HASH_SIZE);
	const float grid_height = HEIGHT / div;
	const float grid_width = WIDTH / div;
	float xi = 0;
	float yi = 0;

	for (size_t i = 0; i < HASH_SIZE; i++)
	{
		this->table[i].rect.x = xi;
		this->table[i].rect.y = yi;
		this->table[i].rect.width = grid_width;
		this->table[i].rect.height = grid_height;
		this->table[i].center = {xi + grid_width / 2, yi + grid_height / 2};
		this->table[i].boids = nullptr;
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
	t_boid_list *tmp = nullptr;
	t_boid_list *next = nullptr;

	for (size_t i = 0; i < HASH_SIZE; i++)
	{
		tmp = this->table[i].boids;
		while (tmp != nullptr)
		{
			next = tmp->next;
			delete tmp;
			tmp = next;
		}
		this->table[i].boids = nullptr;
	}
}

// MEMBER FUNCTIONS
void	SpatialHashing::clear(void)
{
	t_boid_list *tmp = nullptr;
	t_boid_list *next = nullptr;

	for (size_t i = 0; i < HASH_SIZE; i++)
	{
		tmp = this->table[i].boids;
		while (tmp != nullptr)
		{
			next = tmp->next;
			delete tmp;
			tmp = next;
		}
		this->table[i].boids = nullptr;
	}
}

int		SpatialHashing::hash(Vector2 center)
{
	const float hash_grid = sqrtf(HASH_SIZE);
	const float x = floor(center.x / (WIDTH / hash_grid));
	const float y = floor(center.y / (HEIGHT / hash_grid));
	const float result = x + y * hash_grid;

	if (result <= 0)
		return (0);
	else if (result >= HASH_SIZE)
		return (HASH_SIZE - 1);
	return (static_cast<int>(result));
}

void	SpatialHashing::insert(Boid *boid)
{
	const int	index = this->hash(boid->properties.pos);
	t_boid_list	*new_boid;

	new_boid = new t_boid_list;
	new_boid->boid = boid;
	if (this->table[index].boids == nullptr)
		new_boid->next = nullptr;
	else
		new_boid->next = this->table[index].boids;
	this->table[index].boids = new_boid;
}

void	SpatialHashing::draw_rect(Rectangle rect, Color color)
{
	DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, color);
}

void	SpatialHashing::draw(Camera2D camera)
{
	for (size_t i = 0; i < HASH_SIZE; i++)
	{
		if (this->table[i].boids == nullptr)
		{
			if (this->table[i].center.x > (camera.target.x - camera.offset.x / camera.zoom)
				&& this->table[i].center.x < (camera.target.x + camera.offset.x / camera.zoom)
				&& this->table[i].center.y > (camera.target.y - camera.offset.y / camera.zoom)
				&& this->table[i].center.y < (camera.target.y + camera.offset.y / camera.zoom))
				this->draw_rect(this->table[i].rect, GRAY);
		}
	}
	for (size_t i = 0; i < HASH_SIZE; i++)
	{
		if (this->table[i].boids != nullptr)
		{
			if (this->table[i].center.x > (camera.target.x - camera.offset.x / camera.zoom)
				&& this->table[i].center.x < (camera.target.x + camera.offset.x / camera.zoom)
				&& this->table[i].center.y > (camera.target.y - camera.offset.y / camera.zoom)
				&& this->table[i].center.y < (camera.target.y + camera.offset.y / camera.zoom))
				this->draw_rect(this->table[i].rect, GREEN);
		}
		t_boid_list *tmp = this->table[i].boids;
		while (tmp != nullptr)
		{
			if (this->table[i].center.x > (camera.target.x - camera.offset.x / camera.zoom)
				&& this->table[i].center.x < (camera.target.x + camera.offset.x / camera.zoom)
				&& this->table[i].center.y > (camera.target.y - camera.offset.y / camera.zoom)
				&& this->table[i].center.y < (camera.target.y + camera.offset.y / camera.zoom))
				DrawText(std::to_string(i).c_str(), this->table[i].center.x, this->table[i].center.y, 10, BLACK);
			tmp = tmp->next;
		}
	}
}
