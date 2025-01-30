#include "../includes/game.hpp"

void	init_engine(void)
{
	InitWindow(WIDTH, HEIGHT, "Flocking Simulation");
	SetTargetFPS(75);
	rlImGuiSetup(true);
}

void	set_values(Flock *flok, float perception, float max_speed, t_check_box check)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		flok->boids[i].perception = perception;
		flok->boids[i].max_speed = max_speed;
    flok->boids[i].check = check;
	}
}

void	render_imgui(Flock *flok)
{
	float	perception = flok->boids[0].perception;
	float	max_speed = flok->boids[0].max_speed;
  t_check_box check = flok->boids[0].check;

	rlImGuiBegin();
	ImGui::Begin("Flock Settings");
	ImGui::Text("Flock Settings");
	ImGui::SliderFloat("Perception", &perception, 0, 100);
	ImGui::SliderFloat("Max Steer", &max_speed, 0, 10);
  ImGui::Separator();
	ImGui::Text("Display Options");
  ImGui::Checkbox("Draw Boids", &check.draw);
  ImGui::Checkbox("Draw Perception", &check.draw_perception);
  ImGui::Checkbox("Draw Velocity", &check.draw_velocity);
	set_values(flok, perception, max_speed, check);
	ImGui::End();
	rlImGuiEnd();
}

void	update_engine(Flock *flok)
{
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawFPS(10, 10);
		flok->mirror();
		flok->align();
		flok->update();
		flok->draw();
		render_imgui(flok);
		EndDrawing();
	}
}

void	end_engine(void)
{
	rlImGuiShutdown();
	CloseWindow();
}

int	main(void)
{
	Flock	flok;

	init_engine();
	update_engine(&flok);
	end_engine();
	return (0);
}
