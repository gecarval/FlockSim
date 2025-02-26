#include "../includes/game.hpp"

void	set_random_values(Flock *flock)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		flock->boids[i].stats.pos = {static_cast<float>(GetRandomValue(0,
					CANVAS_WIDTH)), static_cast<float>(GetRandomValue(0, CANVAS_HEIGHT))};
		flock->boids[i].vel = {static_cast<float>(GetRandomValue(-flock->boids[i].stats.max_speed,
					flock->boids[i].stats.max_speed)),
			static_cast<float>(GetRandomValue(-flock->boids[i].stats.max_speed,
					flock->boids[i].stats.max_speed))};
	}
}

void	set_values(Flock *flock, t_boid stats, t_check_box check)
{
	t_boid	save;

	flock->check = check;
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		save = flock->boids[i].stats;
		flock->boids[i].stats.min_speed = stats.min_speed;
		flock->boids[i].stats.max_speed = stats.max_speed;
		flock->boids[i].stats.max_alignment = stats.max_alignment;
	}
}

void	render_boid_imguiwindow_lifestats(t_boid *boid)
{
	ImGui::SetNextWindowPos(ImVec2(CANVAS_WIDTH / 2 - 100, CANVAS_HEIGHT / 2 - 100), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_FirstUseEver);
	ImGui::Begin("Boid Stats", NULL, ImGuiWindowFlags_NoResize);
	ImGui::Text("Life Stats");
	ImGui::Text("Boid %d", boid->id);
	ImGui::Text("Health: %.2f", boid->life.health);
	ImGui::Text("Energy: %.2f", boid->life.energy);
	ImGui::Text("Age: %.2f", boid->life.age);
	ImGui::Text("Generation: %d", boid->life.generation);
	ImGui::Text("Children: %d", boid->life.children);
	ImGui::Text("Food: %.2f", boid->life.food);
	ImGui::Text("Alive: %d", boid->life.alive);
	ImGui::Separator();
	ImGui::Text("Boid Stats");
	ImGui::Text("Perception: %.2f", boid->perception);
	ImGui::Text("Max Steer: %.2f", boid->max_steer);
	ImGui::Text("Max Cohesion: %.2f", boid->max_cohesion);
	ImGui::Text("Max Separation: %.2f", boid->max_separation);
	ImGui::Text("Separation Ratio: %.2f", boid->separation_ratio);
	ImGui::Text("Obstacle Avoidance: %.2f", boid->obstacle_avoidance);
	ImGui::Text("Apetite: %.2f", boid->apetite);
	ImGui::Text("Max Speed into Food: %.2f", boid->max_speed_food);
	ImGui::Checkbox("Smells Food", &boid->life.smell);
	ImGui::Separator();
	ImGui::Separator();
	ImGui::Text("Boid stats");
	ImGui::SliderFloat("Perception", &boid->perception, 0, MAX_PERCEPTION);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("The perception radius of each boid.");
	ImGui::SliderFloat("Alignment Ratio", &boid->max_steer, 0, MAX_ALIGN);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Shows the maximum steering force of the alignment force.\nThe force that makes boids follow each other.");
	ImGui::SliderFloat("Max Cohesion", &boid->max_cohesion, 0, MAX_COHESE);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Shows the maximum influence of the cohesion force.\nThe force that makes boids stick together.");
	ImGui::SliderFloat("Max Separation", &boid->max_separation, 0, MAX_SEPARATE);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Shows the maximum influence of the separation force.\nthe force that makes boids avoid each other when they get too close.");
	ImGui::SliderFloat("Separation Ratio", &boid->separation_ratio, 0, MAX_SEPARATION_RATIO);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Shows the ratio of the separation force.\nThe ratio is based on the perception radius of the boid.");
	ImGui::SliderFloat("Obstacle Avoidance", &boid->obstacle_avoidance, 0, MAX_OBSTACLE_AVOIDANCE);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("The limit of force of the obstacle avoidance force.");
	ImGui::End();
}

void	render_imgui(t_game *game)
{
	t_boid		stats;
	t_check_box	check;
	static float		gamespeedmult = 1.0f;
	static float		ingameseconds = 0.0f;

	stats = game->flock.boids[0].stats;
	if (game->player.focus == true)
		stats = game->player.focused_boid->stats;
	check = game->flock.check;
	rlImGuiBegin();
	ImGui::Begin("Flock Settings");
	ImGui::Text("Flocking stats");
	ImGui::Text("In Game Minutes: %.2f", ingameseconds / 60);
	ImGui::Text("Number of Boids: %d", game->flock.options.boids_alive);
	ImGui::Text("Number of Food: %d", game->flock.options.food_amount);
	ImGui::SliderFloat("Min Speed", &stats.min_speed, 0, 10);
	ImGui::SliderFloat("Max Speed", &stats.max_speed, 0, 10);
	ImGui::SliderFloat("Max Alignment", &stats.max_alignment, 0, 10);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Shows the maximum influence of the alignment force.");
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
	if (ImGui::BeginPopupModal("Warning", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (CANVAS_WIDTH + CANVAS_HEIGHT > 20000)
		{
			ImGui::Text("Are you sure you want to draw the hash table?\nThis will slow down the simulation.");
		}
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
	ImGui::Checkbox("Show FPS", &game->flock.options.show_fps);
	ImGui::Checkbox("Mirror", &game->flock.options.mirror);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("The boids will wrap around the canvas.");
	ImGui::Checkbox("Avoid Border", &game->flock.options.avoidborder);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("The boids will avoid the borders of the canvas.");
	ImGui::Checkbox("Align", &game->flock.options.align);
	ImGui::Checkbox("Cohese", &game->flock.options.cohese);
	ImGui::Checkbox("Separate", &game->flock.options.separate);
	ImGui::InputInt("Frame Rate Limit Number", &game->frame_limit);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Caps the max frame rate of the engine.");
	if (ImGui::Button("Limit Frame Rate") == true)
		SetTargetFPS(game->frame_limit);
	ImGui::SliderFloat("GameSpeed", &gamespeedmult, 0.1, 10.0);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("The speed of the flock simulation.\nPlease Note that high speeds may be unstable.");
	ImGui::SliderInt("Alignment Algorithm", &game->flock.options.alignAlgorithm, 0, 2);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("The boids will alignment algorithm.\n0-No Alignment\n1-n^2 Alignment iterating each one with every other\n2-nlogn Alignment iterating each one with the hash.");
	if (ImGui::Button("Randomize Boids Position") == true)
		set_random_values(&game->flock);
	ImGui::Separator();
	ImGui::Text("Food Spawn Settings");
	ImGui::Checkbox("Draw Spawn", &game->spawn.draw);
	ImGui::Checkbox("Spawn on Collision", &game->spawn.oncollision);
	ImGui::Checkbox("Spawn Rectangule | Circle", &game->spawn.active);
	ImGui::Text("Circle Spawn Settings");
	ImGui::SliderFloat("Circle Radius", &game->spawn.circle.radius, 0, CANVAS_WIDTH * 0.75);
	ImGui::SliderFloat("Circle Center X", &game->spawn.circle.pos.x, 0, CANVAS_WIDTH);
	ImGui::SliderFloat("Circle Center Y", &game->spawn.circle.pos.y, 0, CANVAS_HEIGHT);
	ImGui::Text("Rectangle Spawn Settings");
	ImGui::SliderFloat("Rectangle Width", &game->spawn.rect.width, 0, CANVAS_WIDTH);
	ImGui::SliderFloat("Rectangle Height", &game->spawn.rect.height, 0, CANVAS_HEIGHT);
	ImGui::SliderFloat("Rectangle Beginning X", &game->spawn.rect.x, 0, CANVAS_WIDTH);
	ImGui::SliderFloat("Rectangle Beginning Y", &game->spawn.rect.y, 0, CANVAS_HEIGHT);
	ImGui::Separator();
	set_values(&game->flock, stats, check);
	game->flock.options.gamespeed =  (gamespeedmult * 15.0f);
	if (game->pause == false)
		ingameseconds += GetFrameTime() * (game->flock.options.gamespeed / 15.0f);
	ImGui::End();
	if (game->player.focus == true)
		render_boid_imguiwindow_lifestats(&game->player.focused_boid->stats);
	rlImGuiEnd();
}
