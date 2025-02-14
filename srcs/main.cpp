#include "../includes/game.hpp"

void	init_engine(t_game *game)
{
	game->player.camera.target = (Vector2){ WIDTH / 2.0f, HEIGHT / 2.0f };
    game->player.camera.offset = (Vector2){ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f };
    game->player.camera.rotation = 0.0f;
    game->player.camera.zoom = 1.0f;
	game->flock = Flock();
	game->pause = false;
	game->frame_limit = 60;
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Flocking Simulation");
	game->flock.gethash();
	game->textcolor = {0, 0, 0, 125};
    game->texture.hashmap = LoadRenderTexture(WIDTH, HEIGHT);
    game->flock.hash.renderhashmaptexture(game->texture.hashmap);
	SetTargetFPS(game->frame_limit);
	rlImGuiSetup(true);
}

void	set_random_values(Flock *flock)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		flock->boids[i].properties.pos = {static_cast<float>(GetRandomValue(0,
					WIDTH)), static_cast<float>(GetRandomValue(0, HEIGHT))};
		flock->boids[i].vel = {static_cast<float>(GetRandomValue(-flock->boids[i].properties.max_speed,
					flock->boids[i].properties.max_speed)),
			static_cast<float>(GetRandomValue(-flock->boids[i].properties.max_speed,
					flock->boids[i].properties.max_speed))};
	}
}

void	set_values(Flock *flock, t_boid properties, t_check_box check)
{
	t_boid	save;

	flock->check = check;
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		save = flock->boids[i].properties;
		flock->boids[i].properties = properties;
		flock->boids[i].properties.pos = save.pos;
	}
}

void	update_flock(t_game *game)
{
	if (game->flock.options.mirror == true)
		game->flock.mirror();
	if (game->flock.options.avoidborder == true)
		game->flock.avoidborder();
	if (game->flock.options.separate == true || game->flock.options.align == true
		|| game->flock.options.cohese == true)
	{
		if (game->flock.options.alignAlgorithm == 0)
			;
		else if (game->flock.options.alignAlgorithm == 1)
			game->flock.average();
		else if (game->flock.options.alignAlgorithm == 2)
		{
			game->flock.hash.clear();
			game->flock.gethash();
			game->flock.hashaverage();
		}
	}
	if (game->flock.options.separate == true)
		game->flock.separate();
	if (game->flock.options.align == true)
		game->flock.align();
	if (game->flock.options.cohese == true)
		game->flock.cohese();
	game->flock.update();
}

void	render_imgui(t_game *game)
{
	t_boid		properties;
	t_check_box	check;

    properties = game->flock.boids[0].properties;
    check = game->flock.check;
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
    set_values(&game->flock, properties, check);
    ImGui::End();
    rlImGuiEnd();
}

void	engine_input(t_game *game)
{
	const float walkspeed = (0.8f * GetFrameTime()) / (game->player.camera.zoom * 20);
	if (GetMouseWheelMove() != 0)
	{
		const float zoomdir = (GetMouseWheelMove() > 0) ? 1.0f : -1.0f;
		const Vector2 calc = Vector2Scale(Vector2Subtract(GetMousePosition(), game->player.camera.offset), zoomdir);
		const Vector2 mousepos = Vector2Limit(calc, 10.0f);
		game->player.camera.target = Vector2Add(game->player.camera.target,	mousepos);
		game->player.camera.zoom += ((float)GetMouseWheelMove()) * 0.05f;
	}
	if (game->player.camera.zoom > 3.0f)
		game->player.camera.zoom = 3.0f;
    else if (game->player.camera.zoom < 0.1f)
		game->player.camera.zoom = 0.1f;
	if (IsKeyDown(KEY_Q))
		game->player.camera.rotation++;
	else if (IsKeyDown(KEY_E))
		game->player.camera.rotation--;
	if (IsKeyDown(KEY_W))
		game->player.camera.target.y -= (HEIGHT * walkspeed);
	if (IsKeyDown(KEY_A))
		game->player.camera.target.x -= (WIDTH * walkspeed);
	if (IsKeyDown(KEY_S))
		game->player.camera.target.y += (HEIGHT * walkspeed);
	if (IsKeyDown(KEY_D))
		game->player.camera.target.x += (WIDTH * walkspeed);
	if (IsKeyPressed(KEY_R))
		game->player.camera.rotation = 0.0f;
	if (IsKeyPressed(KEY_SPACE))
		game->pause = !game->pause;
	if (game->pause == true)
		if (IsKeyPressed(KEY_RIGHT))
			update_flock(game);
}

void	update_engine(t_game *game)
{
	while (!WindowShouldClose())
	{
		engine_input(game);
		if (game->pause == false)
			update_flock(game);
		BeginDrawing();
		ClearBackground(RAYWHITE);
		BeginMode2D(game->player.camera);
		game->flock.draw(game->player.camera, game->texture.hashmap);
		EndMode2D();
		if (game->flock.options.show_fps == true)
			DrawFPS(10, 10);
		render_imgui(game);
		if (game->pause == true)
		{
			DrawText("PAUSED", WIDTH / 2 - 122, HEIGHT / 2, 50, game->textcolor);
			DrawText("Press SPACE to resume", WIDTH / 2 - 110, HEIGHT / 2 + 50, 15, game->textcolor);
			DrawText("Press RIGHT to update by steps", WIDTH / 2 - 140, HEIGHT / 2 + 75, 15, game->textcolor);
		}
		EndDrawing();
	}
}

void	end_engine(t_game *game)
{
	game->flock.~Flock();
    UnloadRenderTexture(game->texture.hashmap);
	rlImGuiShutdown();
	CloseWindow();
}

int	main(void)
{
	t_game		game;

	init_engine(&game);
	update_engine(&game);
	end_engine(&game);
	return (0);
}
