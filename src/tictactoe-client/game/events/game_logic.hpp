#include "../eventsystem.hpp"

using event = class game::event::ievent;

class game_logic : public event
{
public:
	game_logic( const char *name ) : game::event::ievent( name ) { }

	void on_trigger( void *context ) override;
};