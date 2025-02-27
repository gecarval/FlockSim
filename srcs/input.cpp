#include "../includes/game.hpp"

void	focus_boid_on_click(t_game *game)
{
	t_boid_list		*list = NULL;
	const Vector2	mouseposworld = GetScreenToWorld2D(GetMousePosition(), game->player.camera);
	const int		mousehash = game->flock.hash.hash(mouseposworld);

	list = game->flock.hash.table[mousehash].boids;
	while (list != NULL)
	{
		if (CheckCollisionCircles(mouseposworld, 5.0f, list->boid->stats.pos, list->boid->radius) == true &&
				list->boid->stats.life.alive == true)
		{
			game->player.focused_boid = list->boid;
			game->player.focus = true;
			break ;
		}
		list = list->next;
	}
}

void	engine_input_event_handler(t_game *game)
{
	if (game->player.focus == true)
		game->player.camera.target = game->player.focused_boid->stats.pos;
	if (game->player.shifting == true)
		game->player.camera.target = Vector2Add(game->player.camera.target,
				Vector2Scale(Vector2Subtract(GetMousePosition(), game->player.camera.offset), 0.05f));
}

void	spawn_boid(t_game *game)
{
	for (size_t i = 0; i < NB_BOIDS; i++)
	{
		if (game->flock.boids[i].stats.life.alive == false)
		{
			game->flock.boids[i] = Boid(game->flock.boids[0].stats);
			game->flock.boids[i].stats.id = i + 1;
			game->flock.boids[i].stats.life.alive = true;
			game->flock.boids[i].stats.pos = GetScreenToWorld2D(GetMousePosition(), game->player.camera);
			game->flock.boids[i].vel = {static_cast<float>(GetRandomValue(-game->flock.boids[i].stats.max_speed,
						game->flock.boids[i].stats.max_speed)),
				static_cast<float>(GetRandomValue(-game->flock.boids[i].stats.max_speed,
						game->flock.boids[i].stats.max_speed))};
			game->flock.options.boids_alive++;
			break ;
		}
	}
}

void	engine_input(t_game *game)
{
	const float		walkspeed = (0.8f * GetFrameTime()) / (game->player.camera.zoom * 20);
	static const float	zoommin = 0.1f / ((float)CANVAS_WIDTH / 5000.0f);

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		focus_boid_on_click(game);
	if (IsKeyPressed(KEY_F))
		game->player.focus = false;
	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
		spawn_boid(game);
	if (IsKeyDown(KEY_LEFT_SHIFT) && game->player.focus == false)
		game->player.shifting = true;
	else
		game->player.shifting = false;
	game->player.camera.zoom += ((float)GetMouseWheelMove() * 0.1f * game->player.camera.zoom);
	if (game->player.camera.zoom > 3.0f)
		game->player.camera.zoom = 3.0f;
    else if (game->player.camera.zoom < zoommin)
		game->player.camera.zoom = zoommin;
	if (IsKeyDown(KEY_Q))
		game->player.camera.rotation++;
	else if (IsKeyDown(KEY_E))
		game->player.camera.rotation--;
	if (IsKeyDown(KEY_W))
		game->player.camera.target.y -= (CANVAS_HEIGHT * walkspeed);
	if (IsKeyDown(KEY_A))
		game->player.camera.target.x -= (CANVAS_WIDTH * walkspeed);
	if (IsKeyDown(KEY_S))
		game->player.camera.target.y += (CANVAS_HEIGHT * walkspeed);
	if (IsKeyDown(KEY_D))
		game->player.camera.target.x += (CANVAS_WIDTH * walkspeed);
	if (IsKeyPressed(KEY_R))
		game->player.camera.rotation = 0.0f;
	if (IsKeyPressed(KEY_SPACE))
		game->pause = !game->pause;
	if (game->pause == true)
		if (IsKeyPressed(KEY_RIGHT))
			update_flock(game);
	engine_input_event_handler(game);
}
