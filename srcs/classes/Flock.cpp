#include "../../includes/classes/Flock.hpp"

// CONSTRUCTOR
Flock::Flock()
{
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		this->boids[i] = Boid();
		this->boids[i].stats.id = i + 1;
	}
	this->options = {true, true, true, true, true, true, 2, 15};
	this->check = {true, false, false, false};
	this->hash = SpatialHashing();
}

// DESTRUCTOR
Flock::~Flock()
{
	this->hash.~SpatialHashing();
}

void Flock::average(void)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i].getaverage(this->boids);
}

// MEMBER FUNCTIONS
void Flock::hashaverage(void)
{
	int			hash;
	int			x;
	int			y;
	int			ncheckonx;
	int			ncheckony;
	const int	hash_grid = sqrt(HASH_LEN);
	float		total;
	float		total_avoid;
	t_boid_list	*tmp;

	hash = 0;
	tmp = nullptr;
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
				tmp = this->hash.table[hash + x + (y * hash_grid)].boids;
				while (tmp != nullptr)
				{
					if (&this->boids[i] == tmp->boid)
					{
						tmp = tmp->next;
						continue ;
					}
					if (CheckCollisionCircles(this->boids[i].stats.pos,
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
					total++;
					if (CheckCollisionCircles(this->boids[i].stats.pos,
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
		if (this->options.mirror == true)
			this->boids[i].mirror();
		if (this->options.avoidborder == true)
			this->boids[i].avoidborder();
	}
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		if (this->options.separate == true)
			this->boids[i].separate();
		if (this->options.align == true)
			this->boids[i].align();
		if (this->options.cohese == true)
			this->boids[i].cohese();
		this->boids[i].update(this->options.gamespeed);
		this->boids[i].lifestatsupdate();
	}
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
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->hash.insert(&this->boids[i]);
}

void Flock::lifeupdate(void)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i].lifestatsupdate();
}

void Flock::draw(Camera2D camera, RenderTexture2D texture)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		// this is also considering the camera.zoom
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
		if (this->check.draw_hash == true)
            this->hash.drawhashmaptexture(texture);
		if (this->check.draw_hash == true)
			this->hash.draw(camera);
	}
}
