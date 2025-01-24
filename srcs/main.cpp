#include "../includes/game.hpp"

void flock_align(Boid *flock)
{
	for (int i = 0; i < NB_BOIDS; i++)
	{
		flock[i].align(flock);
	}
}

void flock_update(Boid *flock)
{
	for (int i = 0; i < NB_BOIDS; i++)
	{
		flock[i].update();
	}
}

void flock_draw(Boid *flock)
{
	for (int i = 0; i < NB_BOIDS; i++)
	{
		flock[i].draw();
	}
}

void init_engine()
{
	InitWindow(WIDTH, HEIGHT, "Water Ball");
	SetTargetFPS(75);
	rlImGuiSetup(true);
}

void update_engine(Boid *flock)
{
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawFPS(10, 10);
		flock_align(flock);
		flock_update(flock);
		flock_draw(flock);
		//rlImGuiBegin();
		//bool show_demo_window = true;
		//ImGui::ShowDemoWindow(&show_demo_window);
		//rlImGuiEnd();
		EndDrawing();
	}
}

void end_engine()
{
//	rlImGuiShutdown();
	CloseWindow();
}

int	main(void)
{
	Boid flock[NB_BOIDS];

	init_engine();
	update_engine(flock);
	end_engine();
	return (0);
}
