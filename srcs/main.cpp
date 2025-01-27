#include "../includes/game.hpp"

void init_engine()
{
	InitWindow(WIDTH, HEIGHT, "Water Ball");
	SetTargetFPS(75);
	//rlImGuiSetup(true);
}

void update_engine(Flock flok)
{
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawFPS(10, 10);
		flok.mirror();
		flok.align();
		flok.update();
		flok.draw();
		//rlImGuiBegin();
		//bool show_demo_window = true;
		//ImGui::ShowDemoWindow(&show_demo_window);
		//rlImGuiEnd();
		EndDrawing();
	}
}

void end_engine()
{
	//rlImGuiShutdown();
	CloseWindow();
}

int	main(void)
{
	Flock flok;

	init_engine();
	update_engine(flok);
	end_engine();
	return (0);
}
