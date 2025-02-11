#include "../includes/game.hpp"

bool	pause = false;

void	init_engine(void)
{
	InitWindow(WIDTH, HEIGHT, "Flocking Simulation");
	//	SetTargetFPS(75);
	rlImGuiSetup(true);
}

void	set_random_values(Flock *flok)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		flok->boids[i].properties.pos = {static_cast<float>(GetRandomValue(0,
					WIDTH)), static_cast<float>(GetRandomValue(0, HEIGHT))};
		flok->boids[i].vel = {static_cast<float>(GetRandomValue(-flok->boids[i].properties.max_speed,
					flok->boids[i].properties.max_speed)),
			static_cast<float>(GetRandomValue(-flok->boids[i].properties.max_speed,
					flok->boids[i].properties.max_speed))};
	}
}

void	set_values(Flock *flok, t_boid properties, t_check_box check)
{
	t_boid	save;

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
	if (flok->options.separate == true || flok->options.align == true
		|| flok->options.cohese == true)
	{
		if (flok->options.alignAlgorithm == 0)
			;
		else if (flok->options.alignAlgorithm == 1)
			flok->average();
		else if (flok->options.alignAlgorithm == 2)
		{
			flok->hash.clear();
			flok->gethash();
			flok->hashaverage();
		}
	}
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
	t_boid		properties;
	t_check_box	check;

	properties = flok->boids[0].properties;
	check = flok->check;
	rlImGuiBegin();
	ImGui::Begin("Flock Settings");
	ImGui::Text("Boid Properties");
	ImGui::SliderFloat("Perception", &properties.perception, 0, 200);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("The perception radius of each boid.");
	ImGui::SliderFloat("Min Speed", &properties.min_speed, 0, 10);
	ImGui::SliderFloat("Max Speed", &properties.max_speed, 0, 10);
	ImGui::SliderFloat("Obstacle Avoidance", &properties.obstacle_avoidance, 0,
		0.1);
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
	if (ImGui::Button("Draw Hash Table"))
		ImGui::OpenPopup("Warning");
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Draws the hash table of the boids.");
	if (ImGui::BeginPopupModal("Warning", nullptr,
			ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Are you sure you want to draw the hash table?\nThis will slow down the simulation.");
		if (ImGui::Button("Enable"))
		{
			check.draw_hash = true;
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Disable"))
		{
			check.draw_hash = false;
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
	ImGui::Separator();
	ImGui::Text("Other Options");
	ImGui::Checkbox("Show FPS", &flok->options.show_fps);
	ImGui::Checkbox("Mirror", &flok->options.mirror);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("The boids will wrap around the screen and avoid going out of bounds.");
	ImGui::SliderInt("Alignment Algorithm", &flok->options.alignAlgorithm, 0,
		2);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("The boids will alignment algorithm.\n0-No Alignment\n1-n^2 Alignment iterating each one with every other\n2-nlogn Alignment iterating each one with the hash.");
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
	Color	color;

	color = {0, 0, 0, 125};
	while (!WindowShouldClose())
	{
		engine_input(flok);
		if (pause == false)
			update_flock(flok);
		BeginDrawing();
		ClearBackground(RAYWHITE);
		flok->draw();
		if (flok->options.show_fps == true)
			DrawFPS(10, 10);
		render_imgui(flok);
		if (pause == true)
		{
			DrawText("PAUSED", WIDTH / 2 - 122, HEIGHT / 2, 50, color);
			DrawText("Press SPACE to resume", WIDTH / 2 - 110, HEIGHT / 2 + 50, 15, color);
			DrawText("Press RIGHT to update by steps", WIDTH / 2 - 140, HEIGHT / 2 + 75, 15, color);
		}
		EndDrawing();
	}
}

void	end_engine(Flock *flok)
{
	flok->~Flock();
	rlImGuiShutdown();
	CloseWindow();
}

int	main(void)
{
	Flock	flok;

	flok = Flock();
	init_engine();
	update_engine(&flok);
	end_engine(&flok);
	return (0);
}
