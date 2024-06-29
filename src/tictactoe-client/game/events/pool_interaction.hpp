#include "../eventsystem.hpp"

using event = class game::event::ievent;

class pool_interaction : public event
{
public:
	pool_interaction( const char *name ) : game::event::ievent( name ) { }

	void on_trigger( void *context ) override;
};