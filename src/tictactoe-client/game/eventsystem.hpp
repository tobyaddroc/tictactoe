#pragma once

#include "game.hpp"
#include <vector>

#define POOL_INTERACTION 0
#define GAME_COMMUNICATOR 1
#define GAME_LOGIC 2

namespace game::event
{
	class ievent
	{
	protected:
		const char *event_name;
	public:
		ievent( const char *name ) : event_name( name ) { }
		inline const char *get_event_name( void )
		{
			return this->event_name;
		}

		virtual void on_trigger( void *context ) = 0;
	};

	void setup( );

	extern std::vector< ievent * > container;
}