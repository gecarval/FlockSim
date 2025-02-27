#include "../../includes/classes/Boid.hpp"

// CONSTRUCTOR
Boid::Boid(void)
{
	this->rotation = 0;
	this->radius = BOID_SIZE;
	this->stats = (t_boid){(t_lifestats){MAX_HEALTH, MIN_BIRTH_ENERGY, 0.0f, 0, 0, 0, true, false},
		0, Vector2Zero(), RED, 50, 0.0, 10, 0.06, 5, 0.035, 0.5, 0.35, 0.1, 0.1, 2};
	this->stats.color = (Color){static_cast<unsigned char>(Remap(this->stats.max_steer, 0.1, 0, 0, 255)),
					static_cast<unsigned char>(Remap(this->stats.perception, 0, 200, 0, 255)),
					static_cast<unsigned char>(Remap(this->stats.max_steer, 0, 0.1, 0, 255)), 255};
	this->stats.pos = {static_cast<float>(GetRandomValue(0, CANVAS_WIDTH)),
		static_cast<float>(GetRandomValue(0, CANVAS_HEIGHT))};
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
	this->vel = Vector2Zero();
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
	if (this->stats.pos.x > CANVAS_WIDTH)
		this->stats.pos.x = 0;
	else if (this->stats.pos.x < 0)
		this->stats.pos.x = CANVAS_WIDTH;
	if (this->stats.pos.y > CANVAS_HEIGHT)
		this->stats.pos.y = 0;
	else if (this->stats.pos.y < 0)
		this->stats.pos.y = CANVAS_HEIGHT;
}

void Boid::runfromborder(void)
{
	Vector2 border = {0, 0};
	if (this->stats.pos.x < this->stats.perception)
		border.x = RUNBORDER * (this->stats.perception - this->stats.pos.x);
	else if (this->stats.pos.x > CANVAS_WIDTH - this->stats.perception)
		border.x = -RUNBORDER * (this->stats.pos.x - (CANVAS_WIDTH - this->stats.perception));
	if (this->stats.pos.y < this->stats.perception)
		border.y = RUNBORDER * (this->stats.perception - this->stats.pos.y);
	else if (this->stats.pos.y > CANVAS_HEIGHT - this->stats.perception)
		border.y = -RUNBORDER * (this->stats.pos.y - (CANVAS_HEIGHT - this->stats.perception));
	this->acc = Vector2Add(this->acc, border);
}

void Boid::avoidborder(void)
{
	Vector2 border = {0, 0};
	if (this->stats.pos.x < this->stats.perception)
		border.x = this->stats.obstacle_avoidance * (this->stats.perception - this->stats.pos.x);
	else if (this->stats.pos.x > CANVAS_WIDTH - this->stats.perception)
		border.x = -this->stats.obstacle_avoidance * (this->stats.pos.x - (CANVAS_WIDTH - this->stats.perception));
	if (this->stats.pos.y < this->stats.perception)
		border.y = this->stats.obstacle_avoidance * (this->stats.perception - this->stats.pos.y);
	else if (this->stats.pos.y > CANVAS_HEIGHT - this->stats.perception)
		border.y = -this->stats.obstacle_avoidance * (this->stats.pos.y - (CANVAS_HEIGHT - this->stats.perception));
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

bool Boid::lifestatsupdate(int *boids_alive, Boid *boids, float gamespeed)
{
	const float		foodtoenergy = FOOD_CONSUMPTION * GetFrameTime() * gamespeed;
	this->stats.life.age += GetFrameTime() * (gamespeed / 15);
	if (this->stats.life.energy <= 0)
		this->stats.life.health -= STARVATION_DAMAGE * GetFrameTime() * gamespeed;
	if ((this->stats.life.health <= 0 || this->stats.life.age > 90) &&
			this->stats.life.alive == true)
	{
		this->stats.life.alive = false;
		*boids_alive -= 1;
		return (true);
	}
	if (this->stats.life.energy < MAX_ENERGY - foodtoenergy && this->stats.life.food > 0)
	{
		this->stats.life.food -= foodtoenergy / 2;
		this->stats.life.energy += foodtoenergy;
	}
	else if (this->stats.life.energy > 0)
		this->stats.life.energy -= foodtoenergy;
	if (this->stats.life.energy > MIN_BIRTH_ENERGY &&
			this->stats.life.age > MIN_BIRTH_AGE && *boids_alive < NB_BOIDS)
	{
		for (size_t i = 0; i < NB_BOIDS; i++)
		{
			if (boids[i].stats.life.alive == false)
			{
				boids[i] = this->procreate(boids_alive);
				break ;
			}
		}
	}
	if (this->stats.life.energy < 0)
		this->stats.life.energy = 0;
	if (this->stats.life.food < 0)
		this->stats.life.food = 0;
	return (false);
}

t_boid Boid::tweakstats(t_boid stats)
{
	t_boid newstats = stats;
	newstats.life = (t_lifestats){MAX_HEALTH, MIN_BIRTH_ENERGY, 0.0f,
					stats.life.generation + 1, 0, 0, true, stats.life.smell};
	newstats.perception = stats.perception + GetRandomValue(-20, 20);
	newstats.perception = Clamp(newstats.perception, 0, MAX_PERCEPTION);
	newstats.max_steer = stats.max_steer +
		static_cast<float>(GetRandomValue(-MUTATION_RATE, MUTATION_RATE)) / 1000.0f;
	newstats.max_steer = Clamp(newstats.max_steer, 0, MAX_ALIGN);
	newstats.max_cohesion = stats.max_cohesion +
		static_cast<float>(GetRandomValue(-MUTATION_RATE, MUTATION_RATE)) / 1000.0f;
	newstats.max_cohesion = Clamp(newstats.max_cohesion, 0, MAX_COHESE);
	newstats.max_separation = stats.max_separation +
		static_cast<float>(GetRandomValue(-MUTATION_RATE, MUTATION_RATE)) / 100.0f;
	newstats.max_separation = Clamp(newstats.max_separation, 0, MAX_SEPARATE);
	newstats.separation_ratio = stats.separation_ratio +
		static_cast<float>(GetRandomValue(-MUTATION_RATE, MUTATION_RATE)) / 100.0f;
	newstats.separation_ratio = Clamp(newstats.separation_ratio, 0, MAX_SEPARATION_RATIO);
	newstats.obstacle_avoidance = stats.obstacle_avoidance +
		static_cast<float>(GetRandomValue(-MUTATION_RATE, MUTATION_RATE)) / 1000.0f;
	newstats.obstacle_avoidance = Clamp(newstats.obstacle_avoidance, 0, MAX_OBSTACLE_AVOIDANCE);
	if (this->stats.life.smell == true)
		newstats.apetite = stats.apetite + GetRandomValue(-MUTATION_RATE, MUTATION_RATE) / 100.0f;
	newstats.apetite = Clamp(newstats.apetite, 0, MAX_APETITE);
	if (this->stats.life.smell == true)
		newstats.max_speed_food = stats.max_speed_food +
			(float)GetRandomValue(-MUTATION_RATE, MUTATION_RATE) / 10.0f;
	newstats.max_speed_food = Clamp(newstats.max_speed_food, 0, MAX_SPEED_FOOD);
	// Get Color based on genes
	newstats.color = (Color){static_cast<unsigned char>(Remap(newstats.max_steer, MAX_ALIGN, 0, 0, 255)),
					static_cast<unsigned char>(Remap(newstats.perception, 0, MAX_PERCEPTION, 0, 255)),
					static_cast<unsigned char>(Remap(newstats.max_steer, 0, MAX_ALIGN, 0, 255)), 255};
	return (newstats);
}

Boid Boid::procreate(int *boids_alive)
{
	t_boid stats = this->tweakstats(this->stats);

	this->stats.life.energy -= MIN_BIRTH_ENERGY;
	this->stats.life.children += 1;
	*boids_alive += 1;
	return Boid(stats);
}
