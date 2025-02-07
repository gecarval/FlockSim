#include "../includes/game.hpp"

bool pause = false;

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

	flok->check = check;
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		save = flok->boids[i].properties;
		flok->boids[i].properties = properties;
		flok->boids[i].properties.pos = save.pos;
	}
}

void	update_flock(Flock *flok)
{
	if (flok->options.mirror == true)
		flok->mirror();
	if (flok->options.separate == true || flok->options.align == true || flok->options.cohese == true)
		flok->average();
	if (flok->options.separate == true)
		flok->separate();
	if (flok->options.align == true)
		flok->align();
	if (flok->options.cohese == true)
		flok->cohese();
	flok->update();
}

void	render_imgui(Flock *flok)
{
	t_boid properties = flok->boids[0].properties;
	t_check_box check = flok->check;

	rlImGuiBegin();
	ImGui::Begin("Flock Settings");
	ImGui::Text("Boid Properties");
	ImGui::SliderFloat("Perception", &properties.perception, 0, 200);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("The perception radius of each boid.");
	ImGui::SliderFloat("Min Speed", &properties.min_speed, 0, 10);
	ImGui::SliderFloat("Max Speed", &properties.max_speed, 0, 10);
	ImGui::SliderFloat("Obstacle Avoidance", &properties.obstacle_avoidance, 0, 0.1);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("The limit of force of the obstacle avoidance force.");
	ImGui::Separator();
	ImGui::Text("Flocking Properties");
	ImGui::SliderFloat("Max Alignment", &properties.max_alignment, 0, 10);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Shows the maximum influence of the alignment force.");
	ImGui::SliderFloat("Alignment Ratio", &properties.max_steer, 0, 0.1);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Shows the maximum steering force of the alignment force.\nThe force that makes boids follow each other.");
	ImGui::SliderFloat("Max Cohesion", &properties.max_cohesion, 0, 0.1);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Shows the maximum influence of the cohesion force.\nThe force that makes boids stick together.");
	ImGui::SliderFloat("Max Separation", &properties.max_separation, 0, 1);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Shows the maximum influence of the separation force.\nthe force that makes boids avoid each other when they get too close.");
	ImGui::SliderFloat("Separation Ratio", &properties.separation_ratio, 0, 1);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Shows the ratio of the separation force.\nThe ratio is based on the perception radius of the boid.");
	ImGui::Separator();
	ImGui::Text("Display Options");
	ImGui::Checkbox("Draw Boids", &check.draw);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Draws the boids on the screen.");
	ImGui::Checkbox("Draw Perception", &check.draw_perception);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Draws the perception radius of each boid.");
	ImGui::Checkbox("Draw Forces", &check.draw_velocity);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Draws the Vector of all forces of each boid.");
	ImGui::Separator();
	ImGui::Text("Other Options");
	ImGui::Checkbox("Show FPS", &flok->options.show_fps);
	ImGui::Checkbox("Mirror", &flok->options.mirror);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("The boids will wrap around the screen and avoid going out of bounds.");
	ImGui::Checkbox("Align", &flok->options.align);
	ImGui::Checkbox("Cohese", &flok->options.cohese);
	ImGui::Checkbox("Separate", &flok->options.separate);
	if (ImGui::Button("Randomize Boids Position") == true)
		set_random_values(flok);
	set_values(flok, properties, check);
	ImGui::End();
	rlImGuiEnd();
}

void	engine_input(Flock *flok)
{
	if (IsKeyPressed(KEY_F1))
		flok->options.show_fps = !flok->options.show_fps;
	if (IsKeyPressed(KEY_F2))
		flok->options.mirror = !flok->options.mirror;
	if (IsKeyPressed(KEY_F3))
		flok->options.align = !flok->options.align;
	if (IsKeyPressed(KEY_F4))
		flok->options.cohese = !flok->options.cohese;
	if (IsKeyPressed(KEY_F5))
		flok->options.separate = !flok->options.separate;
	if (IsKeyPressed(KEY_SPACE))
		pause = !pause;
	if (pause == true)
		if (IsKeyPressed(KEY_RIGHT))
			update_flock(flok);
}

void	update_engine(Flock *flok)
{
	while (!WindowShouldClose())
	{
		engine_input(flok);
		BeginDrawing();
		ClearBackground(RAYWHITE);
		if (flok->options.show_fps == true)
			DrawFPS(10, 10);
		if (pause == false)
			update_flock(flok);
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
