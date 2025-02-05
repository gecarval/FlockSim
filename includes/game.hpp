#ifndef GAME_HPP
# define GAME_HPP

// INCLUDES
# include "./classes/Boid.hpp"
# include "./classes/Flock.hpp"
# include "imgui.h"
# include "rlImGui.h"
# include <cmath>
# include <iostream>

// DATA STRUCTURES
// MAIN GAME ENGINE DATA STRUCTURE
typedef struct s_game
{
	double_t      delta_time;
	unsigned int	frame_count;
	unsigned int	fps;
	unsigned int	score;
}					t_game;

// FUNCTION PROTOTYPES

#endif
