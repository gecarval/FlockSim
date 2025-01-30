#include "../../includes/classes/Flock.hpp"

// CONSTRUCTOR
Flock::Flock()
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i] = Boid();
}

// DESTRUCTOR
Flock::~Flock()
{
}

// MEMBER FUNCTIONS
void Flock::align()
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i].align(this->boids);
}

void Flock::update()
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i].update();
}

void Flock::mirror()
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i].mirror();
}

void Flock::draw()
{
	for (size_t i = 0; i < NB_BOIDS; i++)
		this->boids[i].draw();
}
