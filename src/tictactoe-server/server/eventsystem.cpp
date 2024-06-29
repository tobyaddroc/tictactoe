#include "eventsystem.hpp"

#include "events/network_listener.hpp"

std::vector< game::event::ievent * > game::event::container = { };

void game::event::setup( )
{
	container.push_back( new network_dispatcher( "network_dispatcher" ) );
}