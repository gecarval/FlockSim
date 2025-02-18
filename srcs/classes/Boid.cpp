#include "../../includes/classes/Boid.hpp"

// CONSTRUCTOR
Boid::Boid(void)
{
	this->rotation = 0;
	this->radius = BOID_SIZE;
	this->stats = (t_boid){(t_lifestats){100.0f, 1000.0f, 0.0f, 0, 1, 0},
		0, Vector2Zero(), GREEN, 50, 0.0, 10, 0.06, 5, 0.035, 0.5, 0.35, 0.1};
	this->stats.pos = {static_cast<float>(GetRandomValue(0, WIDTH)),
		static_cast<float>(GetRandomValue(0, HEIGHT))};
	this->vel = {static_cast<float>(GetRandomValue(-this->stats.max_speed, this->stats.max_speed)),
		static_cast<float>(GetRandomValue(-this->stats.max_speed, this->stats.max_speed))};
	this->acc = Vector2Zero();
	this->average = {Vector2Zero(), Vector2Zero(), Vector2Zero()};
}

Boid::Boid(t_boid stats)
{
	this->rotation = 0;
	this->radius = BOID_SIZE;
	this->stats = stats;
	const float max_speed = this->stats.max_speed;
	this->vel = {static_cast<float>(GetRandomValue(-max_speed, max_speed)),
		static_cast<float>(GetRandomValue(-max_speed, max_speed))};
	this->acc = Vector2Zero();
	this->average = {Vector2Zero(), Vector2Zero(), Vector2Zero()};
}

// DESTRUCTOR
Boid::~Boid(void)
{
}

// MEMBER FUNCTIONS
void Boid::draw_boid(void)
{
	this->rotation = atan2(this->vel.y, this->vel.x) + PI / 2;
    DrawTriangle(Vector2Add(this->stats.pos, Vector2Rotate({0, -this->radius * 2.5f}, this->rotation)),
        Vector2Add(this->stats.pos, Vector2Rotate({-this->radius, this->radius * 1.5f}, this->rotation)),
        Vector2Add(this->stats.pos, Vector2Rotate({this->radius, this->radius * 1.5f}, this->rotation)),
        this->stats.color);
}

void Boid::draw_perception(void)
{
	const float radius = this->stats.perception;
	DrawCircleLines(this->stats.pos.x, this->stats.pos.y, radius, BLUE);
	const float avoid_radius = radius * this->stats.separation_ratio;
	DrawCircleLines(this->stats.pos.x, this->stats.pos.y, avoid_radius, RED);
}

void Boid::draw_velocity(void)
{
	const float line_length = 1 * this->radius;
	const Vector2 line = Vector2Add(this->stats.pos, this->vel * line_length);
	DrawLine(this->stats.pos.x, this->stats.pos.y, line.x, line.y, GREEN);
}

void Boid::draw_align(void)
{
	const float line_length = 100 * this->radius;
	const Vector2 line = Vector2Add(this->stats.pos, this->average.vel * line_length);
	DrawLine(this->stats.pos.x, this->stats.pos.y, line.x, line.y, BLUE);
}

void Boid::draw_avoid(void)
{
	const float line_length = 15 * this->radius;
	const Vector2 line = Vector2Add(this->stats.pos, this->average.sep * line_length);
	DrawLine(this->stats.pos.x, this->stats.pos.y, line.x, line.y, RED);
}

void Boid::draw_cohese(void)
{
	const float line_length = 40 * this->radius;
	const Vector2 line = Vector2Add(this->stats.pos, this->average.pos * line_length);
	DrawLine(this->stats.pos.x, this->stats.pos.y, line.x, line.y, PINK);
}

void Boid::getaverage(Boid *flock)
{
	float		total;
	float		total_avoid;

	total = 0;
	total_avoid = 0;
	this->average.vel = Vector2Zero();
	this->average.pos = Vector2Zero();
	this->average.sep = Vector2Zero();
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		if (this == &flock[i])
			continue ;
		if (CheckCollisionCircles(this->stats.pos, this->stats.perception, flock[i].stats.pos, flock[i].radius) == false)
			continue ;
		this->average.vel = Vector2Add(this->average.vel, flock[i].vel);
		this->average.pos = Vector2Add(this->average.pos, flock[i].stats.pos);
		total += 1;
		if (CheckCollisionCircles(this->stats.pos, this->stats.perception * this->stats.separation_ratio, flock[i].stats.pos, flock[i].radius) == false)
			continue ;
		this->average.sep = Vector2Add(this->average.sep, Vector2Subtract(this->stats.pos, flock[i].stats.pos));
		total_avoid += 1;
	}
	if (total <= 0)
		return ;
	this->average.vel = Vector2Divide(this->average.vel, {total, total});
	this->average.pos = Vector2Divide(this->average.pos, {total, total});
	if (total_avoid <= 0)
		return ;
	this->average.sep = Vector2Divide(this->average.sep, {total_avoid, total_avoid});
}

void Boid::separate(void)
{
	if (this->average.sep.x == 0 && this->average.sep.y == 0)
		return ;
	this->average.sep = Vector2Normalize(this->average.sep);
	this->average.sep = Vector2Scale(this->average.sep, this->stats.max_separation);
	this->acc = Vector2Add(this->acc, this->average.sep);
}

void Boid::align(void)
{
	if (this->average.vel.x == 0 && this->average.vel.y == 0)
		return ;
	this->average.vel = Vector2Normalize(this->average.vel);
	this->average.vel = Vector2Scale(this->average.vel, this->stats.max_alignment);
	this->average.vel = Vector2Subtract(this->average.vel, this->vel);
	const float length = Vector2Length(this->average.vel);
	if (length != 0 && length > this->stats.max_steer)
		this->average.vel = Vector2Scale(Vector2Normalize(this->average.vel), this->stats.max_steer);
	this->acc = Vector2Add(this->acc, this->average.vel);
}

void Boid::cohese(void)
{
	if (this->average.pos.x == 0 && this->average.pos.y == 0)
		return ;
	this->average.pos = Vector2Subtract(this->average.pos, this->stats.pos);
	this->average.pos = Vector2Normalize(this->average.pos);
	this->average.pos = Vector2Scale(this->average.pos, this->stats.max_cohesion);
	this->acc = Vector2Add(this->acc, this->average.pos);
}

void Boid::mirror(void)
{
	if (this->stats.pos.x > WIDTH)
		this->stats.pos.x = 1;
	else if (this->stats.pos.x < 0)
		this->stats.pos.x = WIDTH - 1;
	if (this->stats.pos.y > HEIGHT)
		this->stats.pos.y = 1;
	else if (this->stats.pos.y < 0)
		this->stats.pos.y = HEIGHT - 1;
}

void Boid::avoidborder(void)
{
	Vector2 border = {0, 0};
	if (this->stats.pos.x < this->stats.perception)
		border.x = this->stats.obstacle_avoidance * (this->stats.perception - this->stats.pos.x);
	else if (this->stats.pos.x > WIDTH - this->stats.perception)
		border.x = -this->stats.obstacle_avoidance * (this->stats.pos.x - (WIDTH - this->stats.perception));
	if (this->stats.pos.y < this->stats.perception)
		border.y = this->stats.obstacle_avoidance * (this->stats.perception - this->stats.pos.y);
	else if (this->stats.pos.y > HEIGHT - this->stats.perception)
		border.y = -this->stats.obstacle_avoidance * (this->stats.pos.y - (HEIGHT - this->stats.perception));
	this->acc = Vector2Add(this->acc, border);
}

void Boid::update(float gamespeed)
{
	this->vel = Vector2Min(this->vel, this->stats.min_speed);
	this->vel = Vector2Limit(this->vel, this->stats.max_speed);
	this->vel = Vector2Add(this->vel, Vector2Scale(this->acc, GetFrameTime() * gamespeed));
	this->stats.pos = Vector2Add(this->stats.pos, Vector2Scale(this->vel, GetFrameTime() * gamespeed));
	this->acc = Vector2Zero();
}

void Boid::lifestatsupdate(void)
{
	const float		foodtoenergy = 50.0f * GetFrameTime();
	if (this->stats.life.energy < 1000 - foodtoenergy && this->stats.life.food > 0)
	{
		this->stats.life.food -= foodtoenergy;
		this->stats.life.energy += foodtoenergy;
	}
	else if (this->stats.life.energy > 0)
		this->stats.life.energy -= foodtoenergy;
	this->stats.life.age += GetFrameTime();
	if (this->stats.life.energy <= 0)
		this->stats.life.health -= 3.0f * GetFrameTime();
	if (this->stats.life.health <= 0)
	{
		this->stats.life.health = 100;
		this->stats.life.energy = 1000;
		this->stats.life.age = 0;
		this->stats.life.generation += 1;
		this->stats.life.children = 0;
		this->stats.life.food = 1000;
	}
}
