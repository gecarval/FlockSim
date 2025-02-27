#ifndef GAME_HPP
# define GAME_HPP

// MACROS
const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 900;

// INCLUDES
# include "./classes/Boid.hpp"
# include "./classes/Flock.hpp"
# include "./classes/SpatialHashing.hpp"
# include "imgui.h"
# include "rlImGui.h"
# include <cmath>
# include <iostream>

// DATA STRUCTURES
typedef struct s_spawn
{
	Circle		circle;
	Rectangle	rect;
	bool		draw;
	bool		active;
	bool		oncollision;
}					t_spawn;

typedef struct s_player
{
	Camera2D	camera;
	Boid		*focused_boid;
	bool		focus;
	bool		shifting;
}					t_player;

typedef struct s_texture2d
{
    RenderTexture2D hashmap;
}					t_texture2d;

// MAIN GAME ENGINE DATA STRUCTURE
typedef struct s_game
{
	t_player	player;
	t_spawn		spawn;
    t_texture2d texture;
    Color       textcolor;
	Flock		flock;
	double		ingameseconds;
	bool		pause;
	int			frame_limit;
}					t_game;

// FUNCTION PROTOTYPES
void	update_flock(t_game *game);
void	engine_draw(t_game *game);
void	engine_drawin_camera(t_game *game);
void	engine_input(t_game *game);
void	render_imgui(t_game *game);

#endif
