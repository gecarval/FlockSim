#include "../../includes/classes/SpatialHashing.hpp"

// CONSTRUCTOR
SpatialHashing::SpatialHashing(void)
{
	const float div = HASH_CALC;
	const float grid_height = CANVAS_HEIGHT / div;
	const float grid_width = CANVAS_WIDTH / div;
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
		this->table[i].food = nullptr;
		if (xi + grid_width >= CANVAS_WIDTH)
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
	t_food_list *tmp_food = nullptr;
	t_food_list *next_food = nullptr;

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
		tmp_food = this->table[i].food;
		while (tmp_food != nullptr)
		{
			next_food = tmp_food->next;
			delete tmp_food;
			tmp_food = next_food;
		}
		this->table[i].food = nullptr;
	}
}

// MEMBER FUNCTIONS
void SpatialHashing::clear(void)
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

int SpatialHashing::hash(Vector2 center)
{
	const float hash_grid = HASH_CALC;
	const float x = floor(center.x / (CANVAS_WIDTH / hash_grid));
	const float y = floor(center.y / (CANVAS_HEIGHT / hash_grid));
	const float result = x + y * hash_grid;

	if (result <= 0)
		return (0);
	else if (result >= HASH_LEN)
		return (HASH_LEN - 1);
	return (static_cast<int>(result));
}

void SpatialHashing::insert(Boid *boid)
{
	const int index = this->hash(boid->stats.pos);
	t_boid_list *new_boid;

	new_boid = new t_boid_list;
	new_boid->boid = boid;
	if (this->table[index].boids == nullptr)
		new_boid->next = nullptr;
	else
		new_boid->next = this->table[index].boids;
	this->table[index].boids = new_boid;
}

void SpatialHashing::insert(t_food *food)
{
	const int index = this->hash(food->pos);
	t_food_list *new_food;

	new_food = new t_food_list;
	new_food->food = food;
	if (this->table[index].food == nullptr)
		new_food->next = nullptr;
	else
		new_food->next = this->table[index].food;
	this->table[index].food = new_food;
}

void SpatialHashing::draw_rect(Rectangle rect, Color color)
{
	DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, color);
}

void SpatialHashing::renderhashmaptexture(RenderTexture2D texture)
{
	BeginTextureMode(texture);
	for (size_t i = 0; i < HASH_LEN; i++)
	{
		this->draw_rect(this->table[i].rect, WHITE);
		DrawText(std::to_string(i).c_str(), this->table[i].center.x,
				 this->table[i].center.y, 10, WHITE);
	}
	EndTextureMode();
}

void SpatialHashing::drawhashmaptexture(RenderTexture2D texture)
{
	DrawTextureRec(texture.texture,
				   (Rectangle){0, 0, (float)texture.texture.width,
							   (float)-texture.texture.height},
				   (Vector2){0, 0}, DARKGRAY);
}

void SpatialHashing::draw(Camera2D camera)
{
	t_boid_list *tmp;
	int amount;

	amount = 0;
	for (size_t i = 0; (i < HASH_LEN && CANVAS_WIDTH + CANVAS_HEIGHT > 20000);
		 i++)
	{
		if (this->table[i].center.x <
				(camera.target.x - camera.offset.x / camera.zoom) ||
			this->table[i].center.x >
				(camera.target.x + camera.offset.x / camera.zoom) ||
			this->table[i].center.y <
				(camera.target.y - camera.offset.y / camera.zoom) ||
			this->table[i].center.y >
				(camera.target.y + camera.offset.y / camera.zoom))
			continue;
		this->draw_rect(this->table[i].rect, DARKGRAY);
		DrawText(std::to_string(i).c_str(), this->table[i].center.x,
				 this->table[i].center.y, 10, DARKGRAY);
	}
	tmp = nullptr;
	for (size_t i = 0; i < HASH_LEN; i++)
	{
		if (this->table[i].center.x <
				(camera.target.x - camera.offset.x / camera.zoom) ||
			this->table[i].center.x >
				(camera.target.x + camera.offset.x / camera.zoom) ||
			this->table[i].center.y <
				(camera.target.y - camera.offset.y / camera.zoom) ||
			this->table[i].center.y >
				(camera.target.y + camera.offset.y / camera.zoom))
			continue;
		if (this->table[i].boids != nullptr)
		{
			amount = 0;
			tmp = this->table[i].boids;
			while (tmp != nullptr)
			{
				if (tmp->boid->stats.life.alive == true)
					amount++;
				tmp = tmp->next;
			}
			if (amount > 0 && amount <= 6)
				this->draw_rect(this->table[i].rect, YELLOW);
		}
	}
	tmp = nullptr;
	for (size_t i = 0; i < HASH_LEN; i++)
	{
		if (this->table[i].center.x <
				(camera.target.x - camera.offset.x / camera.zoom) ||
			this->table[i].center.x >
				(camera.target.x + camera.offset.x / camera.zoom) ||
			this->table[i].center.y <
				(camera.target.y - camera.offset.y / camera.zoom) ||
			this->table[i].center.y >
				(camera.target.y + camera.offset.y / camera.zoom))
			continue;
		if (this->table[i].boids != nullptr)
		{
			amount = 0;
			tmp = this->table[i].boids;
			while (tmp != nullptr)
			{
				if (tmp->boid->stats.life.alive == true)
					amount++;
				tmp = tmp->next;
			}
			if (amount > 6 && amount <= 16)
				this->draw_rect(this->table[i].rect, ORANGE);
		}
	}
	tmp = nullptr;
	for (size_t i = 0; i < HASH_LEN; i++)
	{
		if (this->table[i].center.x <
				(camera.target.x - camera.offset.x / camera.zoom) ||
			this->table[i].center.x >
				(camera.target.x + camera.offset.x / camera.zoom) ||
			this->table[i].center.y <
				(camera.target.y - camera.offset.y / camera.zoom) ||
			this->table[i].center.y >
				(camera.target.y + camera.offset.y / camera.zoom))
			continue;
		if (this->table[i].boids != nullptr)
		{
			amount = 0;
			tmp = this->table[i].boids;
			while (tmp != nullptr)
			{
				if (tmp->boid->stats.life.alive == true)
					amount++;
				tmp = tmp->next;
			}
			if (amount > 16)
				this->draw_rect(this->table[i].rect, RED);
		}
	}
}
