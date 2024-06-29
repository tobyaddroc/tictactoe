#include "eventsystem.hpp"

#include "events/pool_interaction.hpp"
#include "events/network_listener.hpp"
#include "events/game_logic.hpp"

std::vector< game::event::ievent * > game::event::container = { };

void game::event::setup( )
{
	container.push_back( new pool_interaction( "pool_interaction" ) );
	container.push_back( new network_dispatcher( "network_dispatcher" ) );
	container.push_back( new game_logic( "game_logic" ) );
}