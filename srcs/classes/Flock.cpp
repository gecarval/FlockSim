#include "../../includes/classes/Flock.hpp"

// CONSTRUCTOR
Flock::Flock()
{
	int		i;

	i = 0;
	while (i < NB_BOIDS / 10)
	{
		this->boids[i] = Boid();
		this->boids[i].stats.id = i + 1;
		i++;
	}
	while (i < NB_BOIDS)
	{
		this->boids[i] = Boid();
		this->boids[i].stats.id = i + 1;
		this->boids[i].stats.life.alive = false;
		this->boids[i].stats.life.health = 0;
		i++;
	}
	this->options = {true, true, true, true, true, true, 2, 15, NB_BOIDS / 10, 0};
	this->check = {true, false, false, false};
	this->hash = SpatialHashing();
	this->food = nullptr;
}

// DESTRUCTOR
Flock::~Flock()
{
	this->hash.~SpatialHashing();
}

// MEMBER FUNCTIONS
inline void Flock::hashaverage(void)
{
	int			hash;
	int			x;
	int			y;
	int			ncheckonx;
	int			ncheckony;
	const int	hash_grid = HASH_CALC;
	float		total;
	float		total_avoid;
	int			counter;
	t_boid_list	*tmp;
	t_food_list	*prev_foodlist;
	t_food_list	*foodlist;
	t_food_list *closest_food;

	hash = 0;
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		total = 0;
		total_avoid = 0;
		this->boids[i].average.vel = Vector2Zero();
		this->boids[i].average.pos = Vector2Zero();
		this->boids[i].average.sep = Vector2Zero();
		hash = this->hash.hash(this->boids[i].stats.pos);
		ncheckonx = ceil(this->boids[i].stats.perception
				/ this->hash.table[hash].rect.width);
		ncheckony = ceil(this->boids[i].stats.perception
				/ this->hash.table[hash].rect.height);
		x = -ncheckonx - 1;
		while (++x <= ncheckonx)
		{
			y = -ncheckony - 1;
			while (++y <= ncheckony)
			{
				if (hash + x + (y * hash_grid) < 0 || hash + x + (y
						* hash_grid) >= HASH_LEN)
					continue ;
				counter = 0;
				tmp = nullptr;
				foodlist = nullptr;
				prev_foodlist = nullptr;
				closest_food = nullptr;
				tmp = this->hash.table[hash + x + (y * hash_grid)].boids;
				foodlist = this->hash.table[hash + x + (y * hash_grid)].food;
				while (foodlist != nullptr && this->boids[i].stats.life.food <= 2000 &&
						counter < 2 && CheckCollisionCircleRec(this->boids[i].stats.pos,
						this->boids[i].radius, this->hash.table[hash + x + (y * hash_grid)].rect) == true)
				{
					if (CheckCollisionCircles(this->boids[i].stats.pos,
							this->boids[i].radius,
							foodlist->food->pos,
							foodlist->food->radius + 1) == true)
					{
						this->boids[i].stats.life.food += foodlist->food->energy;
						if (prev_foodlist == nullptr)
							this->hash.table[hash + x + (y * hash_grid)].food = foodlist->next;
						else
							prev_foodlist->next = foodlist->next;
						this->remove_food(foodlist->food);
						delete foodlist;
						foodlist = this->hash.table[hash + x + (y * hash_grid)].food;
						counter++;
						continue ;
					}
					if (CheckCollisionCircles(this->boids[i].stats.pos,
							this->boids[i].stats.perception,
							foodlist->food->pos,
							foodlist->food->radius) == true &&
							this->boids[i].stats.life.smell == true)
					{
						if (closest_food == nullptr)
							closest_food = foodlist;
						else if (Vector2Distance(this->boids[i].stats.pos,
								foodlist->food->pos) <
								Vector2Distance(this->boids[i].stats.pos,
								closest_food->food->pos))
							closest_food = foodlist;
					}
					prev_foodlist = foodlist;
					foodlist = foodlist->next;
				}
				if (closest_food != nullptr && this->boids[i].stats.life.smell == true &&
						this->boids[i].stats.life.food <= 2000)
					this->boids[i].attract_towards(closest_food->food->pos);
				counter = 0;
				while (tmp != nullptr && counter < 5)
				{
					if (&this->boids[i] == tmp->boid ||
						tmp->boid->stats.life.alive == false ||
						CheckCollisionCircles(this->boids[i].stats.pos,
							this->boids[i].stats.perception,
							tmp->boid->stats.pos,
							tmp->boid->radius) == false)
					{
						tmp = tmp->next;
						continue ;
					}
					this->boids[i].average.vel = Vector2Add(this->boids[i].average.vel,
							tmp->boid->vel);
					this->boids[i].average.pos = Vector2Add(this->boids[i].average.pos,
							tmp->boid->stats.pos);
					counter++;
					total++;
					if (tmp->boid->stats.life.age <= 2 ||
							CheckCollisionCircles(this->boids[i].stats.pos,
							this->boids[i].stats.perception
							* this->boids[i].stats.separation_ratio,
							tmp->boid->stats.pos,
							tmp->boid->radius) == false)
					{
						tmp = tmp->next;
						continue ;
					}
					this->boids[i].average.sep = Vector2Add(this->boids[i].average.sep,
							Vector2Subtract(this->boids[i].stats.pos,
								tmp->boid->stats.pos));
					total_avoid++;
					tmp = tmp->next;
				}
			}
		}
		if (total <= 0)
			continue ;
		this->boids[i].average.vel = Vector2Divide(this->boids[i].average.vel,
				{total, total});
		this->boids[i].average.pos = Vector2Divide(this->boids[i].average.pos,
				{total, total});
		if (total_avoid <= 0)
			continue ;
		if (this->boids[i].stats.life.age <= 3)
			this->boids[i].average.sep = Vector2Divide(this->boids[i].average.sep,
				{total_avoid * 3, total_avoid * 3});
		else
			this->boids[i].average.sep = Vector2Divide(this->boids[i].average.sep,
				{total_avoid, total_avoid});
	}
}

void Flock::updateflock(void)
{
	if (this->options.separate == true || this->options.align == true
		|| this->options.cohese == true)
	{
		if (this->options.alignAlgorithm == 0)
			;
		else if (this->options.alignAlgorithm == 1)
			this->average();
		else if (this->options.alignAlgorithm == 2)
		{
			this->hash.clear();
			this->gethash();
			this->hashaverage();
		}
	}
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		if (this->boids[i].stats.life.alive == false)
			continue ;
		if (this->options.mirror == true)
			this->boids[i].mirror();
		if (this->options.avoidborder == true)
			this->boids[i].avoidborder();
		if (this->options.separate == true)
			this->boids[i].separate();
		if (this->options.align == true)
			this->boids[i].align();
		if (this->options.cohese == true)
			this->boids[i].cohese();
	}
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		if (this->boids[i].stats.life.alive == false)
			continue ;
		this->boids[i].update(this->options.gamespeed);
		this->boids[i].lifestatsupdate(&this->options.boids_alive, this->boids, this->options.gamespeed);
	}
}

void Flock::average(void)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i].getaverage(this->boids);
}

void Flock::separate(void)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i].separate();
}

void Flock::cohese(void)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i].cohese();
}

void Flock::align(void)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i].align();
}

void Flock::update(void)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i].update(this->options.gamespeed);
}

void Flock::mirror(void)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i].mirror();
}

void Flock::avoidborder(void)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i].avoidborder();
}

void Flock::gethash(void)
{
	t_food *food;

	for (size_t i = 0; i < NB_BOIDS; i++)
		this->hash.insert(&this->boids[i]);
	food = this->food;
	while (food != nullptr)
	{
		this->hash.insert(food);
		food = food->next;
	}
}

void Flock::lifeupdate(void)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i].lifestatsupdate(&this->options.boids_alive, this->boids, this->options.gamespeed);
}

void Flock::remove_food(t_food *food)
{
	t_food *tmp;
	t_food *prev;

	tmp = this->food;
	prev = nullptr;
	while (tmp != nullptr)
	{
		if (tmp == food)
		{
			if (prev == nullptr)
				this->food = tmp->next;
			else
				prev->next = tmp->next;
			delete tmp;
			this->options.food_amount -= 1;
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void Flock::generate_food(void)
{
	return ;
	t_food *new_food;

	for (size_t i = 0; i < NB_BOIDS / 10; i++)
	{
		new_food = new t_food;
		new_food->pos = {static_cast<float>(GetRandomValue(210, CANVAS_WIDTH - 210)),
						static_cast<float>(GetRandomValue(210, CANVAS_HEIGHT - 210))};
		new_food->radius = FOOD_RADIUS;
		new_food->energy = FOOD_ENERGY;
		if (this->food == nullptr)
			new_food->next = nullptr;
		else
			new_food->next = this->food;
		this->food = new_food;
		this->options.food_amount += 1;
	}
}

bool Flock::generate_one_food(Circle circle, bool oncollision)
{
	t_food *new_food;

	if (this->options.food_amount >= NB_BOIDS)
		return (true);
	const float x = static_cast<float>(GetRandomValue(0, CANVAS_WIDTH));
	const float y = static_cast<float>(GetRandomValue(0, CANVAS_HEIGHT));
	if (CheckCollisionPointCircle({x, y}, circle.pos, circle.radius) == !oncollision)
		return (false);
	new_food = new t_food;
	new_food->pos = {x, y};
	new_food->radius = FOOD_RADIUS;
	new_food->energy = FOOD_ENERGY;
	if (this->food == nullptr)
		new_food->next = nullptr;
	else
		new_food->next = this->food;
	this->food = new_food;
	this->options.food_amount += 1;
	return (true);
}

bool Flock::generate_one_food(Rectangle rect, bool oncollision)
{
	t_food *new_food;

	if (this->options.food_amount >= NB_BOIDS)
		return (true);
	const float x = static_cast<float>(GetRandomValue(0, CANVAS_WIDTH));
	const float y = static_cast<float>(GetRandomValue(0, CANVAS_HEIGHT));
	if (CheckCollisionPointRec({x, y}, rect) == !oncollision)
		return (false);
	new_food = new t_food;
	new_food->pos = {x, y};
	new_food->radius = FOOD_RADIUS;
	new_food->energy = FOOD_ENERGY;
	if (this->food == nullptr)
		new_food->next = nullptr;
	else
		new_food->next = this->food;
	this->food = new_food;
	this->options.food_amount += 1;
	return (true);
}

void Flock::generate_food_overtime(float gamespeed, Circle circle, bool oncollision)
{
	static float timer = 0;

	timer += GetFrameTime() * gamespeed / 15;
	while (timer >= FOOD_GEN)
	{
		if (this->generate_one_food(circle, oncollision) == true)
			timer -= FOOD_GEN;
	}
}

void Flock::generate_food_overtime(float gamespeed, Rectangle rect, bool oncollision)
{
	static float timer = 0;

	timer += GetFrameTime() * gamespeed / 15;
	while (timer >= FOOD_GEN)
	{
		if (this->generate_one_food(rect, oncollision) == true)
			timer -= FOOD_GEN;
	}
}

void Flock::draw(Camera2D camera, RenderTexture2D texture)
{
	t_food *food;

	if (this->check.draw_hash == true)
		this->hash.drawhashmaptexture(texture);
	if (this->check.draw_hash == true)
		this->hash.draw(camera);
	food = this->food;
	while (food != nullptr)
	{
		if (food->pos.x > (camera.target.x - camera.offset.x / camera.zoom)
				&& food->pos.x < (camera.target.x + camera.offset.x / camera.zoom)
				&& food->pos.y > (camera.target.y - camera.offset.y / camera.zoom)
				&& food->pos.y < (camera.target.y + camera.offset.y / camera.zoom))
		DrawCircle(food->pos.x, food->pos.y, food->radius, (Color){32, 160, 32, 255});
		food = food->next;
	}
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		if (this->boids[i].stats.life.alive == false)
			continue ;
		if (this->boids[i].stats.pos.x > (camera.target.x - camera.offset.x / camera.zoom)
				&& this->boids[i].stats.pos.x < (camera.target.x + camera.offset.x / camera.zoom)
				&& this->boids[i].stats.pos.y > (camera.target.y - camera.offset.y / camera.zoom)
				&& this->boids[i].stats.pos.y < (camera.target.y + camera.offset.y / camera.zoom))
		{
			if (this->check.draw == true)
				this->boids[i].draw_boid();
			if (this->check.draw_velocity == true)
			{
				this->boids[i].draw_velocity();
				if (this->options.align == true)
					this->boids[i].draw_align();
				if (this->options.cohese == true)
					this->boids[i].draw_cohese();
				if (this->options.separate == true)
					this->boids[i].draw_avoid();
			}
			if (this->check.draw_perception == true)
				this->boids[i].draw_perception();
		}
	}
}
