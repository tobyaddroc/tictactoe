#include "game.hpp"

#include <algorithm>
#include <vector>
#include <array>

game::pool_t *game::game_pool = new pool_t;
bool game::game_started = false;
game::pool_cage game::turn = game::pool_cage::x;

bool game::check_win( game::pool_cage team )
{
	static const int combinations[ 8 ][ 3 ] = {
		{ 0, 1, 2 },
		{ 3, 4, 5 },
		{ 6, 7, 8 },
		{ 0, 3, 6 },
		{ 1, 4, 7 },
		{ 2, 5, 8 },
		{ 0, 4, 8 },
		{ 2, 4, 6 }
	};

	for ( const auto &combination : combinations )
	{
		short counter = 0;
		for ( const auto &idx : combination )
		{
			if ( game::game_pool->get( )[ idx ] == team )
			{
				counter++;
			}
			if ( counter == 3 )
			{
				return true;
			}
		}
	}

	return false;
}

void game::exit( )
{
	return std::exit( 0 );
}