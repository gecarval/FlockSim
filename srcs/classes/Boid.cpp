#include "../../includes/classes/Boid.hpp"

// CONSTRUCTOR
Boid::Boid(void)
{
	this->rotation = 0;
	this->radius = BOID_SIZE;
	this->stats = (t_boid){(t_lifestats){100.0f, 1000.0f, 0.0f, 0, 0, 0, true},
		0, Vector2Zero(), RED, 50, 0.0, 10, 0.06, 5, 0.035, 0.5, 0.35, 0.1, 0.1, 2};
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
	const float line_length = 1.0f * this->radius;
	const Vector2 line = Vector2Add(this->stats.pos, this->vel * line_length);
	DrawLine(this->stats.pos.x, this->stats.pos.y, line.x, line.y, GREEN);
}

void Boid::draw_align(void)
{
	const float line_length = 100.0f * this->radius;
	const Vector2 line = Vector2Add(this->stats.pos, this->average.vel * line_length);
	DrawLine(this->stats.pos.x, this->stats.pos.y, line.x, line.y, BLUE);
}

void Boid::draw_avoid(void)
{
	const float line_length = 15.0f * this->radius;
	const Vector2 line = Vector2Add(this->stats.pos, this->average.sep * line_length);
	DrawLine(this->stats.pos.x, this->stats.pos.y, line.x, line.y, RED);
}

void Boid::draw_cohese(void)
{
	const float line_length = 40.0f * this->radius;
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
		this->stats.pos.x = WIDTH - 2;
	if (this->stats.pos.y > HEIGHT)
		this->stats.pos.y = 1;
	else if (this->stats.pos.y < 0)
		this->stats.pos.y = HEIGHT - 2;
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

void Boid::attract_towards(Vector2 target)
{
	Vector2 desired = Vector2Subtract(target, this->stats.pos);
	desired = Vector2Normalize(desired);
	desired = Vector2Scale(desired, this->stats.apetite);
	Vector2 steer = desired;
	const float length = Vector2Length(steer);
	if (length != 0 && length > this->stats.max_speed_food)
		steer = Vector2Scale(Vector2Normalize(steer), this->stats.max_speed_food);
	this->acc += Vector2Add(this->acc, steer);
}

void Boid::flee_from(Vector2 target)
{
	Vector2 desired = Vector2Subtract(this->stats.pos, target);
	desired = Vector2Normalize(desired);
	desired = Vector2Scale(desired, this->stats.apetite);
	Vector2 steer = Vector2Subtract(desired, this->vel);
	const float length = Vector2Length(steer);
	if (length != 0 && length > this->stats.max_speed)
		steer = Vector2Scale(Vector2Normalize(steer), this->stats.max_speed);
	this->acc += Vector2Add(this->acc, steer);
}

void Boid::update(float gamespeed)
{
	this->vel = Vector2Min(this->vel, this->stats.min_speed);
	this->vel = Vector2Limit(this->vel, this->stats.max_speed);
	this->vel = Vector2Add(this->vel, Vector2Scale(this->acc, GetFrameTime() * gamespeed));
	this->stats.pos = Vector2Add(this->stats.pos, Vector2Scale(this->vel, GetFrameTime() * gamespeed));
	this->acc = Vector2Zero();
}

void Boid::lifestatsupdate(int *boids_alive, Boid *boids)
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
	if (this->stats.life.health <= 0 && this->stats.life.alive == true)
	{
		this->stats.life.alive = false;
		*boids_alive -= 1;
	}
	if (this->stats.life.energy > 500 && this->stats.life.age > 20 && *boids_alive < NB_BOIDS)
	{
		this->stats.life.energy -= 500;
		this->stats.life.children += 1;
		for (size_t i = 0; i < NB_BOIDS; i++)
		{
			if (boids[i].stats.life.alive == false)
			{
				boids[i] = Boid(this->tweakstats(this->stats));
				break ;
			}
		}
	}
}

t_boid Boid::tweakstats(t_boid stats)
{
	t_boid newstats = stats;
	newstats.life = (t_lifestats){100.0f, 2000.0f, 0.0f,
					stats.life.generation + 1, 0, 0, true};
	newstats.perception = stats.perception + GetRandomValue(-5, 5);
	newstats.max_steer = stats.max_steer + (float)GetRandomValue(-1, 1) / 100.0f;
	if (newstats.max_steer < 0)
		newstats.max_steer = 0;
	newstats.max_cohesion = stats.max_cohesion + (float)GetRandomValue(-1, 1) / 100.0f;
	if (newstats.max_cohesion < 0)
		newstats.max_cohesion = 0;
	newstats.max_separation = stats.max_separation + (float)GetRandomValue(-1, 1) / 100.0f;
	if (newstats.max_separation < 0)
		newstats.max_separation = 0;
	newstats.separation_ratio = stats.separation_ratio + (float)GetRandomValue(-1, 1) / 100.0f;
	if (newstats.separation_ratio < 0)
		newstats.separation_ratio = 0;
	newstats.obstacle_avoidance = stats.obstacle_avoidance + (float)GetRandomValue(-1, 1) / 100.0f;
	if (newstats.obstacle_avoidance < 0)
		newstats.obstacle_avoidance = 0;
//	newstats.apetite = stats.apetite + GetRandomValue(-5, 5) / 100.0f;
//	newstats.max_speed_food = stats.max_speed_food + GetRandomValue(-100, 100) / 10.0f;
	return newstats;
}

Boid Boid::procreate(void)
{
	t_boid stats = this->tweakstats(this->stats);

	this->stats.life.energy -= 500;
	this->stats.life.children += 1;
	return Boid(stats);
}

