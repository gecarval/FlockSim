#include "../../includes/classes/Boid.hpp"

// CONSTRUCTOR
Boid::Boid()
{
	this->properties.color = RED;
	this->sides = 3;
	this->rotation = 0;
	this->radius = BOID_SIZE;
	this->properties.max_speed = 5;
  this->properties.min_speed = 0.5;
  this->properties.max_steer = 0.01;
	this->properties.perception = 50;
	this->properties.check = {true, false, false};
	this->frame_time_counter = 0;
	this->properties.pos = {static_cast<float>(GetRandomValue(0, WIDTH)),
		static_cast<float>(GetRandomValue(0, HEIGHT))};
	this->vel = {static_cast<float>(GetRandomValue(this->properties.max_speed * -1, this->properties.max_speed)),
		static_cast<float>(GetRandomValue(this->properties.max_speed * -1, this->properties.max_speed))};
	this->acc = Vector2Zero();
}

Boid::Boid(t_boid properties)
{
	this->sides = 3;
	this->rotation = 0;
	this->frame_time_counter = 0;
	this->radius = BOID_SIZE;
	this->properties = properties;
	this->vel = {static_cast<float>(GetRandomValue(-this->properties.max_speed, this->properties.max_speed)),
		static_cast<float>(GetRandomValue(-this->properties.max_speed, this->properties.max_speed))};
	this->acc = Vector2Zero();
}

// DESTRUCTOR
Boid::~Boid()
{
}

// MEMBER FUNCTIONS
void Boid::draw()
{
	frame_time_counter += GetFrameTime();
	if (frame_time_counter >= 0.1)
	{
		frame_time_counter -= 0.1;
		this->rotation = atan2(this->vel.y, this->vel.x) * RAD2DEG;
	}
  if (this->properties.check.draw == true)
    DrawPoly(this->properties.pos, this->sides, this->radius, this->rotation, this->properties.color);
  if (this->properties.check.draw_perception == true)
    DrawCircleLines(this->properties.pos.x, this->properties.pos.y, this->properties.perception, GREEN);
  if (this->properties.check.draw_velocity == true)
    DrawLine(this->properties.pos.x, this->properties.pos.y, this->properties.pos.x + this->vel.x * 30, this->properties.pos.y + this->vel.y * 30, BLUE);
}

void Boid::align(Boid *flock)
{
	Vector2	steer;
	int		count;

	steer = Vector2Zero();
	count = 0;
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		if (&this->properties.pos == &flock[i].properties.pos)
			continue ;
		if (CheckCollisionCircles(this->properties.pos, this->properties.perception, flock[i].properties.pos, flock[i].radius) == false)
			continue ;
		steer.x += flock[i].vel.x;
		steer.y += flock[i].vel.y;
		count++;
	}
	if (count == 0)
		return ;
	steer.x /= count;
	steer.y /= count;
  steer = Vector2Subtract(steer, this->vel);
  steer = Vector2Multiply(steer, {this->properties.max_steer, this->properties.max_steer});
  this->acc = Vector2Add(this->acc, steer);
}

void Boid::mirror()
{
	if (this->properties.pos.x > WIDTH)
		this->properties.pos.x = 1;
	else if (this->properties.pos.x < 0)
		this->properties.pos.x = WIDTH - 1;
	if (this->properties.pos.y > HEIGHT)
		this->properties.pos.y = 1;
	else if (this->properties.pos.y < 0)
		this->properties.pos.y = HEIGHT - 1;
}

void Boid::update()
{
	this->properties.pos.x += (this->vel.x * GetFrameTime() * 15);
	this->properties.pos.y += (this->vel.y * GetFrameTime() * 15);
	this->vel.x += this->acc.x;
	this->vel.y += this->acc.y;
	this->acc = Vector2Zero();
  const Vector2 max = {this->properties.max_speed, this->properties.max_speed};
  const Vector2 min = {this->properties.min_speed, this->properties.min_speed};
  this->vel = Vector2Clamp(this->vel, min, max);
}
