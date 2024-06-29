#pragma once

#include "game.hpp"
#include <vector>

namespace game::input
{
	extern std::vector< char > virtual_stdin;

	void setup( );
	void run( );
}