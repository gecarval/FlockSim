#include "../includes/game.hpp"

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
	}
}
