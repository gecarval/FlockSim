#ifndef BOID_HPP
#define BOID_HPP

// INCLUDES
#include "../raylib.h"
#include "../raymath.h"
#include <iostream>

// MACROS
const int CANVAS_WIDTH = 20000;
const int CANVAS_HEIGHT = 20000;
const int NB_BOIDS = 30000;
const int NB_FOOD = NB_BOIDS * 2;
const int BOID_SIZE = 3;
const float RUNBORDER = 1.0f;
// FOOD
const float FOOD_GEN = 0.003f;
const float FOOD_RADIUS = 2.0f;
const int FOOD_ENERGY = 300;
const Color FOOD_COLOR = (Color){32, 160, 32, 255};
// MAX MUTATION STATS
const float MAX_PERCEPTION = 200.0f;
const float MAX_ALIGN = 0.1f;
const float MAX_COHESE = 0.1f;
const float MAX_SEPARATE = 1.0f;
const float MAX_SEPARATION_RATIO = 1.0f;
const float MAX_OBSTACLE_AVOIDANCE = 0.1f;
// LIFE STATS
const float MAX_HEALTH = 100.0f;
const float MAX_ENERGY = 2000.0f;
const float MAX_FOOD = 1000.0f;
const float MAX_AGE = 90.0f;
const float MIN_BIRTH_AGE = 20.0f;
const float MIN_BIRTH_ENERGY = 1000.0f;
const float FOOD_CONSUMPTION = 3.5f;
const float STARVATION_DAMAGE = 0.2f;
const float MAX_APETITE = 0.0f;
const float MAX_SPEED_FOOD = 7.5f;
const int MUTATION_RATE = 5;

// STRUCTURES
typedef struct s_average
{
	Vector2 vel;
	Vector2 pos;
	Vector2 sep;
} t_average;

typedef struct s_lifestats
{
	float health;
	float energy;
	float age;
	int generation;
	int children;
	float food;
	bool alive;
	bool smell;
} t_lifestats;

typedef struct s_boid
{
	t_lifestats life;
	int id;
	Vector2 pos;
	Color color;
	float perception;
	float min_speed;
	float max_speed;
	float max_steer;
	float max_alignment;
	float max_cohesion;
	float max_separation;
	float separation_ratio;
	float obstacle_avoidance;
	float apetite;
	float max_speed_food;
} t_boid;

// CLASS DEFINITIONS
class Boid
{
  protected:
	Vector2 acc;
	float rotation;

  public:
	float radius;
	Vector2 vel;
	t_boid stats;
	t_average average;

  public:
	~Boid(void);
	Boid(void);
	Boid(t_boid stats);
	void getaverage(Boid *flock);
	void separate(void);
	void align(void);
	void cohese(void);
	void update(float gamespeed);
	bool lifestatsupdate(int *boids_alive, Boid *boids, float gamespeed);
	void mirror(void);
	void runfromborder(void);
	void avoidborder(void);
	void attract_towards(Vector2 target);
	void flee_from(Vector2 target);
	t_boid tweakstats(t_boid stats);
	Boid procreate(int *boids_alive);
	void draw_boid(void);
	void draw_perception(void);
	void draw_velocity(void);
	void draw_align(void);
	void draw_cohese(void);
	void draw_avoid(void);
};

#endif
