#include "../../includes/classes/SpatialHashing.hpp"

// CONSTRUCTOR
SpatialHashing::SpatialHashing(void)
{
	const float div = sqrt(HASH_LEN);
	const float grid_height = HEIGHT / div;
	const float grid_width = WIDTH / div;
	float xi = 0;
	float yi = 0;

	for (size_t i = 0; i < HASH_LEN; i++)
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

	for (size_t i = 0; i < HASH_LEN; i++)
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

	for (size_t i = 0; i < HASH_LEN; i++)
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
	const float hash_grid = sqrtf(HASH_LEN);
	const float x = floor(center.x / (WIDTH / hash_grid));
	const float y = floor(center.y / (HEIGHT / hash_grid));
	const float result = x + y * hash_grid;

	if (result <= 0)
		return (0);
	else if (result >= HASH_LEN)
		return (HASH_LEN - 1);
	return (static_cast<int>(result));
}

void	SpatialHashing::insert(Boid *boid)
{
	const int	index = this->hash(boid->stats.pos);
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

void    SpatialHashing::renderhashmaptexture(RenderTexture2D texture)
{
    BeginTextureMode(texture);
    for (size_t i = 0; i < HASH_LEN; i++)
    {
        this->draw_rect(this->table[i].rect, GRAY);
        DrawText(std::to_string(i).c_str(),
                this->table[i].center.x, this->table[i].center.y, 10, BLACK);
    }
    EndTextureMode();
}

void    SpatialHashing::drawhashmaptexture(RenderTexture2D texture)
{
    DrawTextureRec(texture.texture, (Rectangle){0, 0,
            (float)texture.texture.width, (float)-texture.texture.height},
            (Vector2){0, 0}, WHITE);
}

void	SpatialHashing::draw(Camera2D camera)
{
	for (size_t i = 0; i < HASH_LEN; i++)
	{
		if (this->table[i].center.x < (camera.target.x - camera.offset.x / camera.zoom)
				|| this->table[i].center.x > (camera.target.x + camera.offset.x / camera.zoom)
				|| this->table[i].center.y < (camera.target.y - camera.offset.y / camera.zoom)
				|| this->table[i].center.y > (camera.target.y + camera.offset.y / camera.zoom))
			continue ;
		if (this->table[i].boids != nullptr)
			this->draw_rect(this->table[i].rect, GREEN);
	}
}
