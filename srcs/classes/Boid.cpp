#include "../../includes/classes/Boid.hpp"

// CONSTRUCTOR
Boid::Boid()
{
	this->pos = {static_cast<float>(GetRandomValue(0, WIDTH)),
		static_cast<float>(GetRandomValue(0, HEIGHT))};
	this->vel = {static_cast<float>(GetRandomValue(-3, 3)),
		static_cast<float>(GetRandomValue(-3, 3))};
	this->acc = Vector2Zero();
	this->color = RED;
	this->sides = 3;
	this->radius = BOID_SIZE;
	this->rotation = 0;
	this->max_speed = 3;
  this->min_speed = 1;
	this->perception = 50;
	this->check = {true, false, false};
	this->frame_time_counter = 0;
}

Boid::Boid(Vector2 pos, float perception, float min_speed, float max_speed, Color col)
{
	this->pos = pos;
	this->vel = {static_cast<float>(GetRandomValue(-3, 3)),
		static_cast<float>(GetRandomValue(-3, 3))};
	this->acc = Vector2Zero();
	this->color = col;
	this->sides = 3;
	this->radius = BOID_SIZE;
	this->rotation = 0;
	this->max_speed = max_speed;
	this->min_speed = min_speed;
	this->perception = perception;
	this->check = {true, false, false};
	this->frame_time_counter = 0;
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
		frame_time_counter = 0;
		this->rotation = atan2(this->vel.y, this->vel.x) * RAD2DEG;
	}
  if (this->check.draw == true)
    DrawPoly(this->pos, this->sides, this->radius, this->rotation, this->color);
  if (this->check.draw_perception == true)
    DrawCircleLines(this->pos.x, this->pos.y, this->perception, GREEN);
  if (this->check.draw_velocity == true)
    DrawLine(this->pos.x, this->pos.y, this->pos.x + this->vel.x * 30, this->pos.y + this->vel.y * 30, BLUE);
}

void Boid::align(Boid *flock)
{
	Vector2	steer;
	int		count;

	steer = Vector2Zero();
	count = 0;
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		if (&this->pos == &flock[i].pos)
			continue ;
		if (CheckCollisionCircles(this->pos, this->perception, flock[i].pos, flock[i].radius) == false)
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
  const Vector2 steer_limit = {this->max_speed, this->max_speed};
  const Vector2 steer_clamp = {this->min_speed, this->min_speed};
  steer = Vector2Min(steer, steer_clamp);
  steer = Vector2Max(steer, steer_limit);
  this->acc = Vector2Add(this->acc, steer);
}

void Boid::mirror()
{
	if (this->pos.x > WIDTH)
		this->pos.x = 1;
	else if (this->pos.x < 0)
		this->pos.x = WIDTH - 1;
	if (this->pos.y > HEIGHT)
		this->pos.y = 1;
	else if (this->pos.y < 0)
		this->pos.y = HEIGHT - 1;
}

void Boid::update()
{
	this->pos.x += (this->vel.x * GetFrameTime() * 15);
	this->pos.y += (this->vel.y * GetFrameTime() * 15);
	this->vel.x += this->acc.x;
	this->vel.y += this->acc.y;
	this->acc = Vector2Zero();
}
