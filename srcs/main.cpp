#include "../includes/game.hpp"

void	init_engine(t_game *game)
{
	game->player.camera.target = (Vector2){ WIDTH / 2.0f, HEIGHT / 2.0f };
    game->player.camera.offset = (Vector2){ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f };
    game->player.camera.rotation = 0.0f;
    game->player.camera.zoom = 1.0f;
	game->player.focus = false;
	game->player.shifting = false;
	game->player.focused_boid = NULL;
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

void	update_engine(t_game *game)
{
	while (!WindowShouldClose())
	{
		engine_input(game);
		if (game->pause == false)
			game->flock.updateflock();
		BeginDrawing();
		ClearBackground(RAYWHITE);
		BeginMode2D(game->player.camera);
		game->flock.draw(game->player.camera, game->texture.hashmap);
		EndMode2D();
		engine_draw(game);
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
