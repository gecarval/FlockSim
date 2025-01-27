# include "../../includes/classes/Boid.hpp"

// CONSTRUCTOR
Boid::Boid():radius(BOID_SIZE) {
    this->pos = {static_cast<float>(GetRandomValue(0, WIDTH)), static_cast<float>(GetRandomValue(0, HEIGHT))};
    this->vel = {static_cast<float>(GetRandomValue(-3, 3)), static_cast<float>(GetRandomValue(-3, 3))};
    this->acc = Vector2Zero();
    this->color = RED;
}

Boid::Boid(Vector2 pos):radius(BOID_SIZE) {
    this->pos = pos;
    this->vel = Vector2Zero();
    this->acc = Vector2Zero();
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
    Vector2 steer = Vector2Zero();
    int count;

    count = 0;
    for (size_t i = 0; i < NB_BOIDS; i++)
    {
        if (&this->pos == &flock[i].pos)
            continue ;
        if (CheckCollisionCircles(this->pos, perception,
            flock[i].pos, flock[i].radius) == false)
            continue ;
        steer.x += flock[i].vel.x;
        steer.y += flock[i].vel.y;
        count++;
    }
    if (count == 0)
        return ;
    steer.x /= count;
    steer.y /= count;
    steer = Vector2Limit(steer, 50.0);
    this->acc.x += (steer.x - this->vel.x);
    this->acc.y += (steer.y - this->vel.y);
}

void Boid::mirror() {
    if (this->pos.x > WIDTH) {
        this->pos.x = 1;
    }
    else if (this->pos.x < 0) {
        this->pos.x = WIDTH - 1;
    }
    if (this->pos.y > HEIGHT) {
        this->pos.y = 1;
    }
    else if (this->pos.y < 0) {
        this->pos.y = HEIGHT - 1;
    }
}

void Boid::update() {
    this->pos.x += (this->vel.x * GetFrameTime() * 15);
    this->pos.y += (this->vel.y * GetFrameTime() * 15);
    this->vel.x += this->acc.x;
    this->vel.y += this->acc.y;
    this->acc = Vector2Zero();
}
