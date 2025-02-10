#include "../../includes/classes/Flock.hpp"

// CONSTRUCTOR
Flock::Flock()
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i] = Boid();
	this->options = {false, true, true, true, true};
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
	float total;
	float total_avoid;
	t_boid_list *tmp;
	Circle circ;

	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		total = 0;
		total_avoid = 0;
		circ = {this->boids[i].properties.pos, this->boids[i].properties.perception};
		const int hash = this->hash.hash(this->boids[i].properties.pos);
		if (CheckCollisionCircleRec(circ.pos, circ.radius, this->hash.table[hash].rect) == false)
			continue ;
		tmp = this->hash.table[hash].boids;
		while (tmp != nullptr)
		{
			if (&this->boids[i] == tmp->boid)
			{
				tmp = tmp->next;
				continue ;
			}
			if (CheckCollisionCircles(circ.pos, circ.radius, tmp->boid->properties.pos, tmp->boid->radius) == false)
			{
				tmp = tmp->next;
				continue ;
			}
			this->boids[i].average.vel = Vector2Add(this->boids[i].average.vel, tmp->boid->vel);
			this->boids[i].average.pos = Vector2Add(this->boids[i].average.pos, tmp->boid->properties.pos);
			total++;
			if (CheckCollisionCircles(circ.pos, circ.radius * this->boids[i].properties.separation_ratio, tmp->boid->properties.pos, tmp->boid->radius) == false)
			{
				tmp = tmp->next;
				continue ;
			}
			this->boids[i].average.sep = Vector2Add(this->boids[i].average.sep, Vector2Subtract(this->boids[i].properties.pos, tmp->boid->properties.pos));
			total_avoid++;
			tmp = tmp->next;
		}
		if (total <= 0)
			continue ;
		this->boids[i].average.vel = Vector2Divide(this->boids[i].average.vel, {total, total});
		this->boids[i].average.pos = Vector2Divide(this->boids[i].average.pos, {total, total});
		if (total_avoid <= 0)
			continue ;
		this->boids[i].average.sep = Vector2Divide(this->boids[i].average.sep, {total_avoid, total_avoid});
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
		this->boids[i].update();
}

void Flock::mirror(void)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i].mirror();
}

void Flock::gethash(void)
{
	this->hash.clear();
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->hash.insert(&this->boids[i]);
}

void Flock::draw(void)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
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
		if (this->check.draw_hash == true)
			this->hash.draw();
	}
}
