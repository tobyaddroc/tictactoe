#pragma once

#include "../eventsystem.hpp"

using event = class game::event::ievent;

class network_dispatcher : public event
{
public:
	network_dispatcher( const char *name ) : game::event::ievent( name ) { }

	void on_trigger( void *context ) override;
};