#include "../includes/game.hpp"

void	init_engine(t_game *game)
{
	game->player.camera.target = (Vector2){ CANVAS_WIDTH / 2.0f, CANVAS_HEIGHT / 2.0f };
    game->player.camera.offset = (Vector2){ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f };
    game->player.camera.rotation = 0.0f;
    game->player.camera.zoom = 1.0f;
	game->player.focus = false;
	game->player.shifting = false;
	game->player.focused_boid = NULL;
	game->pause = false;
	game->frame_limit = 60;
	game->spawn.draw = false;
	game->spawn.circle = { {CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2}, CANVAS_HEIGHT / 4 };
	game->spawn.rect = { 200, 200, CANVAS_WIDTH - 400, CANVAS_HEIGHT - 400};
	game->spawn.oncollision = true;
	game->spawn.active = false;
	game->ingameseconds = 0;
	game->textcolor = {0, 0, 0, 125};
	game->flock = Flock();
	game->flock.gethash();
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Flocking Simulation");
	SetTargetFPS(game->frame_limit);
	rlImGuiSetup(true);
    game->texture.hashmap = LoadRenderTexture(CANVAS_WIDTH, CANVAS_HEIGHT);
    game->flock.hash.renderhashmaptexture(game->texture.hashmap);
	game->flock.generate_food();
}

void	update_objects(t_game *game)
{
	if (game->pause == false)
	{
		game->flock.updateflock();
		if (game->spawn.active == true)
			game->flock.generate_food_overtime(game->flock.options.gamespeed,
					game->spawn.circle, game->spawn.oncollision);
		else
			game->flock.generate_food_overtime(game->flock.options.gamespeed,
					game->spawn.rect, game->spawn.oncollision);
	}
}

void	update_engine(t_game *game)
{
	while (!WindowShouldClose())
	{
		engine_input(game);
		update_objects(game);
		BeginDrawing();
		ClearBackground({0, 0, 24, 255});
		BeginMode2D(game->player.camera);
		game->flock.draw(game->player.camera, game->texture.hashmap);
		engine_drawin_camera(game);
		EndMode2D();
		engine_draw(game);
		if (game->flock.options.show_fps == true)
			DrawFPS(10, 10);
		render_imgui(game);
		if (game->pause == true)
		{
			DrawText("PAUSED", CANVAS_WIDTH / 2 - 122, CANVAS_HEIGHT / 2, 50, game->textcolor);
			DrawText("Press SPACE to resume", CANVAS_WIDTH / 2 - 110, CANVAS_HEIGHT / 2 + 50, 15, game->textcolor);
			DrawText("Press RIGHT to update by steps", CANVAS_WIDTH / 2 - 140, CANVAS_HEIGHT / 2 + 75, 15, game->textcolor);
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
