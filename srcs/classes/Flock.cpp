#include "../../includes/classes/Flock.hpp"

// CONSTRUCTOR
Flock::Flock()
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i] = Boid();
	this->options = {false, true, true, true, true};
	this->check = {true, false, false};
}

// DESTRUCTOR
Flock::~Flock()
{
}

// MEMBER FUNCTIONS
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
		this->boids[i].update();
}

void Flock::mirror(void)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i].mirror();
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
	}
}
