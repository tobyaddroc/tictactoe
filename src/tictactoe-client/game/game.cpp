#include "game.hpp"

game::pool_t *game::game_pool = new pool_t;
bool game::is_in_game = false;
bool game::connecting = false;
bool game::your_turn = false;
game::pool_cage game::team = game::pool_cage::free;

void game::exit( )
{
	return std::exit( 0 );
}