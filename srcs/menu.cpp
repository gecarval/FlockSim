#include "../includes/game.hpp"

void	set_random_values(Flock *flock)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		flock->boids[i].stats.pos = {static_cast<float>(GetRandomValue(0,
					WIDTH)), static_cast<float>(GetRandomValue(0, HEIGHT))};
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
		flock->boids[i].stats = stats;
		flock->boids[i].stats.pos = save.pos;
	}
}

void	render_imgui(t_game *game)
{
	t_boid		stats;
	t_check_box	check;

    stats = game->flock.boids[0].stats;
    check = game->flock.check;
    rlImGuiBegin();
    ImGui::Begin("Flock Settings");
    ImGui::Text("Boid stats");
    ImGui::SliderFloat("Perception", &stats.perception, 0, 200);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("The perception radius of each boid.");
    ImGui::SliderFloat("Min Speed", &stats.min_speed, 0, 10);
    ImGui::SliderFloat("Max Speed", &stats.max_speed, 0, 10);
    ImGui::SliderFloat("Obstacle Avoidance", &stats.obstacle_avoidance, 0, 0.1);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("The limit of force of the obstacle avoidance force.");
    ImGui::Separator();
    ImGui::Text("Flocking stats");
    ImGui::SliderFloat("Max Alignment", &stats.max_alignment, 0, 10);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Shows the maximum influence of the alignment force.");
    ImGui::SliderFloat("Alignment Ratio", &stats.max_steer, 0, 0.1);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Shows the maximum steering force of the alignment force.\nThe force that makes boids follow each other.");
    ImGui::SliderFloat("Max Cohesion", &stats.max_cohesion, 0, 0.1);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Shows the maximum influence of the cohesion force.\nThe force that makes boids stick together.");
    ImGui::SliderFloat("Max Separation", &stats.max_separation, 0, 1);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Shows the maximum influence of the separation force.\nthe force that makes boids avoid each other when they get too close.");
    ImGui::SliderFloat("Separation Ratio", &stats.separation_ratio, 0, 1);
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
    if (ImGui::BeginPopupModal("Warning", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (WIDTH + HEIGHT > 20000)
        {
            ImGui::Text("The hash table is too big to be drawn.");
            ImGui::Text("Please reduce the size of the canvas.");
        }
        else
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
    ImGui::SliderFloat("GameSpeed", &game->flock.options.gamespeed, 0.1, 200.0);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("The speed of the flock simulation.\nPlease Note that high speeds may be unstable.");
    ImGui::SliderInt("Alignment Algorithm", &game->flock.options.alignAlgorithm, 0, 2);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("The boids will alignment algorithm.\n0-No Alignment\n1-n^2 Alignment iterating each one with every other\n2-nlogn Alignment iterating each one with the hash.");
    if (ImGui::Button("Randomize Boids Position") == true)
        set_random_values(&game->flock);
    set_values(&game->flock, stats, check);
    ImGui::End();
    rlImGuiEnd();
}
