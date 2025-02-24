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

void	engine_input(t_game *game)
{
	const float		walkspeed = (0.8f * GetFrameTime()) / (game->player.camera.zoom * 20);
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		focus_boid_on_click(game);
	if (IsKeyPressed(KEY_F))
		game->player.focus = false;
	if (IsKeyDown(KEY_LEFT_SHIFT) && game->player.focus == false)
		game->player.shifting = true;
	else
		game->player.shifting = false;
	game->player.camera.zoom += ((float)GetMouseWheelMove() * 5.0f * GetFrameTime() *
									game->player.camera.zoom);
	if (game->player.camera.zoom > 3.0f)
		game->player.camera.zoom = 3.0f;
    else if (game->player.camera.zoom < 0.1f)
		game->player.camera.zoom = 0.1f;
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
