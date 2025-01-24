#ifndef GAME_HPP
# define GAME_HPP

# include "raylib.h"
# include "raymath.h"

# include <iostream>
# include <cmath>

# include "imgui.h"
# include "rlImGui.h"
# include "./classes/Boid.hpp"

// MACROS
# define WIDTH 1600
# define HEIGHT 900
# define NB_BOIDS 100
# define STEERING 0.01

// DATA STRUCTURES
// MAIN GAME ENGINE DATA STRUCTURE
typedef struct s_game
{
  double_t delta_time;
  unsigned int frame_count;
  unsigned int fps;
  unsigned int score;
} t_game;

// FUNCTION PROTOTYPES

#endif

