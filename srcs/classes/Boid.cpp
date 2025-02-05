#include "../../includes/classes/Boid.hpp"

// CONSTRUCTOR
Boid::Boid(void)
{
	this->properties.color = RED;
	this->sides = 3;
	this->rotation = 0;
	this->radius = BOID_SIZE;
	this->properties.min_speed = 0.0;
	this->properties.max_speed = 5;
	this->properties.perception = 50;
	this->properties.max_steer = 0.03;
	this->properties.max_cohesion = 0.035;
	this->properties.max_separation = 0.05;
	this->properties.check = {true, true, true};
	this->frame_time_counter = 0;
	this->properties.pos = {static_cast<float>(GetRandomValue(0, WIDTH)),
		static_cast<float>(GetRandomValue(0, HEIGHT))};
	this->vel = {static_cast<float>(GetRandomValue(-this->properties.max_speed, this->properties.max_speed)),
		static_cast<float>(GetRandomValue(-this->properties.max_speed, this->properties.max_speed))};
	this->acc = Vector2Zero();
	this->average = {Vector2Zero(), Vector2Zero(), Vector2Zero()};
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
	this->average = {Vector2Zero(), Vector2Zero(), Vector2Zero()};
}

// DESTRUCTOR
Boid::~Boid(void)
{
}

// MEMBER FUNCTIONS
void Boid::draw(t_globaloptions options)
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
	{
		DrawLine(this->properties.pos.x, this->properties.pos.y, this->properties.pos.x + this->vel.x * 30, this->properties.pos.y + this->vel.y * 30, RED);
		if (options.align == true)
			DrawLine(this->properties.pos.x, this->properties.pos.y, this->properties.pos.x + this->average.vel.x * 3000, this->properties.pos.y + this->average.vel.y * 3000, BLUE);
		if (options.cohese == true)
			DrawLine(this->properties.pos.x, this->properties.pos.y, this->properties.pos.x + this->average.pos.x * 3000, this->properties.pos.y + this->average.pos.y * 3000, YELLOW);
		if (options.separate == true)
			DrawLine(this->properties.pos.x, this->properties.pos.y, this->properties.pos.x + this->average.sep.x * 3000, this->properties.pos.y + this->average.sep.y * 3000, PURPLE);
	}
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
		if (CheckCollisionCircles(this->properties.pos, this->properties.perception, flock[i].properties.pos, flock[i].radius) == false)
			continue ;
		this->average.vel = Vector2Add(this->average.vel, flock[i].vel);
		this->average.pos = Vector2Add(this->average.pos, flock[i].properties.pos);
		total += 1;
		if (CheckCollisionCircles(this->properties.pos, this->properties.perception * 0.65, flock[i].properties.pos, flock[i].radius) == false)
			continue ;
		this->average.sep = Vector2Add(this->average.sep, Vector2Subtract(this->properties.pos, flock[i].properties.pos));
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
	this->average.sep = Vector2Scale(this->average.sep, this->properties.max_separation);
	this->acc = Vector2Add(this->acc, this->average.sep);
}

void Boid::align(void)
{
	float myangle = 0;
	float avarageangle = 0;
	float inter = 0;
	float angle = 0;

	if (this->average.vel.x == 0 && this->average.vel.y == 0)
		return ;
	myangle = atan2(this->vel.y, this->vel.x);
	avarageangle = atan2(this->average.vel.y, this->average.vel.x);
	if (myangle < 0)
		myangle += 2 * PI;
	if (avarageangle < 0)
		avarageangle += 2 * PI;
	if (avarageangle < myangle)
		inter = myangle - avarageangle;
	else
		inter = avarageangle - myangle;
	if (inter >= PI)
		angle = (inter - PI) * -1;
	else
		angle = inter;
	std::cout << "my angle    : " << myangle * RAD2DEG << std::endl;
	std::cout << "other angle : " << avarageangle * RAD2DEG << std::endl;
	std::cout << "target angle: " << angle * RAD2DEG << std::endl;
	std::cout << "vel         : " << this->vel.x << " " << this->vel.y << std::endl;
	std::cout << "average.vel : " << this->average.vel.x << " " << this->average.vel.y << std::endl;
	this->average.vel = Vector2Rotate(this->vel, angle);
	this->average.vel = Vector2Scale(this->average.vel, this->properties.max_steer);
	this->acc = Vector2Add(this->acc, this->average.vel);
	this->average.vel = Vector2Rotate(this->vel, angle);
	this->average.vel = Vector2Scale(this->average.vel, this->properties.max_steer);
	this->acc = Vector2Add(this->acc, this->average.vel);
/*	this->average.vel = Vector2Subtract(this->average.vel, this->vel);
	this->average.vel = Vector2Normalize(this->average.vel);
	this->average.vel = Vector2Scale(this->average.vel, this->properties.max_steer);
	this->acc = Vector2Add(this->acc, this->average.vel);*/
}

void Boid::cohese(void)
{
	if (this->average.pos.x == 0 && this->average.pos.y == 0)
		return ;
	this->average.pos = Vector2Subtract(this->average.pos, this->properties.pos);
	this->average.pos = Vector2Normalize(this->average.pos);
	this->average.pos = Vector2Scale(this->average.pos, this->properties.max_cohesion);
	this->acc = Vector2Add(this->acc, this->average.pos);
}

void Boid::mirror(void)
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

void Boid::update(void)
{
	this->vel = Vector2Min(this->vel, this->properties.min_speed);
	this->vel = Vector2Limit(this->vel, this->properties.max_speed);
	this->vel = Vector2Add(this->vel, this->acc);
	this->properties.pos.x += (this->vel.x * GetFrameTime() * 15);
	this->properties.pos.y += (this->vel.y * GetFrameTime() * 15);
	this->acc = Vector2Zero();
}
