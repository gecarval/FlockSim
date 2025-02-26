#include "../includes/game.hpp"

void	update_flock(t_game *game)
{
	if (game->pause == true)
		return ;
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

void	engine_drawin_camera(t_game *game)
{
	if (game->spawn.draw == true)
	{
		if (game->spawn.active == true)
		{
			DrawCircleLines(game->spawn.circle.pos.x, game->spawn.circle.pos.y,
					game->spawn.circle.radius, RED);
		}
		else
		{
			DrawRectangleLines(game->spawn.rect.x, game->spawn.rect.y,
					game->spawn.rect.width, game->spawn.rect.height, RED);
		}
	}
}

void	engine_draw(t_game *game)
{
	Vector2	focused_boid_pos = Vector2Zero();
	Vector2	focused_boid_world = Vector2Zero();

	if (game->player.focus == true)
		focused_boid_pos = game->player.focused_boid->stats.pos;
	focused_boid_world = GetWorldToScreen2D(focused_boid_pos, game->player.camera);
	if (game->player.focus == true)
	{
		DrawCircleLines(focused_boid_world.x, focused_boid_world.y,
						game->player.focused_boid->radius * 3, RED);
		DrawText("Press F to unfocus", focused_boid_world.x + 10, focused_boid_world.y - 10, 10, RED);
	}
}
