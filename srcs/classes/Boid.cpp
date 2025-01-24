# include "../../includes/classes/Boid.hpp"

// CONSTRUCTOR
Boid::Boid() {
    this->pos = {static_cast<float>(GetRandomValue(0, WIDTH)), static_cast<float>(GetRandomValue(0, HEIGHT))};
    this->vel = {static_cast<float>(GetRandomValue(-1, 1)), static_cast<float>(GetRandomValue(-1, 1))};
    this->acc = {0.7, 0.2};
    this->radius = 5;
    this->color = RED;
}

Boid::Boid(Vector2 pos) {
    this->pos = pos;
    this->vel = Vector2Zero();
    this->acc = Vector2Zero();
    this->radius = 5;
    this->color = RED;
}

// DESTRUCTOR
Boid::~Boid() {
}

// MEMBER FUNCTIONS
void Boid::draw() {
    DrawCircle(this->pos.x, this->pos.y, this->radius, this->color);
}

void Boid::align(Boid *flock) {
    const float perception = 50;
    float distance;
    Vector2 steer = Vector2Zero();
    int count;

    count = 0;
    for (size_t i = 0; i < NB_BOIDS; i++)
    {
        if (this->pos.x == flock[i].pos.x && this->pos.y == flock[i].pos.y)
            continue ;
        distance = sqrt(pow(this->pos.x - flock[i].pos.x, 2) + pow(this->pos.y - flock[i].pos.y, 2));
        if (distance >= perception)
            continue ;
        steer.x += flock[i].vel.x;
        steer.y += flock[i].vel.y;
        count++;
    }
    steer.x /= count;
    steer.y /= count;

    this->acc.x += (steer.x - this->vel.x) * STEERING;
    this->acc.y += (steer.y - this->vel.y) * STEERING;
}

void Boid::update() {
    this->pos.x += (this->vel.x * GetFrameTime() * 15);
    this->pos.y += (this->vel.y * GetFrameTime() * 15);
    this->vel.x += this->acc.x;
    this->vel.y += this->acc.y;
    this->acc.x = 0;
    this->acc.y = 0;
    this->mirror();
}

void Boid::mirror() {
    if (this->pos.x > WIDTH) {
        this->pos.x = 0;
    }
    if (this->pos.x < 0) {
        this->pos.x = WIDTH;
    }
    if (this->pos.y > HEIGHT) {
        this->pos.y = 0;
    }
    if (this->pos.y < 0) {
        this->pos.y = HEIGHT;
    }
}