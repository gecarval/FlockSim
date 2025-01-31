#include "../includes/game.hpp"

void	init_engine(void)
{
	InitWindow(WIDTH, HEIGHT, "Flocking Simulation");
	SetTargetFPS(75);
	rlImGuiSetup(true);
}

void  set_random_values(Flock *flok)
{
  for (size_t i = 0; i < NB_BOIDS; i++)
  {
    flok->boids[i].properties.pos = {static_cast<float>(GetRandomValue(0, WIDTH)),
      static_cast<float>(GetRandomValue(0, HEIGHT))};
    flok->boids[i].vel = {static_cast<float>(GetRandomValue(-flok->boids[i].properties.max_speed, flok->boids[i].properties.max_speed)),
      static_cast<float>(GetRandomValue(-flok->boids[i].properties.max_speed, flok->boids[i].properties.max_speed))};
  }
}

void	set_values(Flock *flok, t_boid properties, t_check_box check)
{
  t_boid save;

	for (size_t i = 0; i < NB_BOIDS; i++)
	{
    save = flok->boids[i].properties;
		flok->boids[i].properties = properties;
    flok->boids[i].properties.pos = save.pos;
    flok->boids[i].properties.check = check;
	}
}

void	render_imgui(Flock *flok)
{
  t_boid properties = flok->boids[0].properties;
  t_check_box check = flok->boids[0].properties.check;

	rlImGuiBegin();
	ImGui::Begin("Flock Settings");
	ImGui::Text("Flock Settings");
	ImGui::SliderFloat("Perception", &properties.perception, 0, 100);
	ImGui::SliderFloat("Max Speed", &properties.max_speed, 0, 10);
  ImGui::SliderFloat("Min Speed", &properties.min_speed, 0, 10);
  ImGui::InputFloat("Max Steer", &properties.max_steer);
  ImGui::Separator();
	ImGui::Text("Display Options");
  ImGui::Checkbox("Draw Boids", &check.draw);
  ImGui::Checkbox("Draw Perception", &check.draw_perception);
  ImGui::Checkbox("Draw Velocity", &check.draw_velocity);
  ImGui::Separator();
  ImGui::Text("Other Options");
  if (ImGui::Button("Randomize") == true)
    set_random_values(flok);
	set_values(flok, properties, check);
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
