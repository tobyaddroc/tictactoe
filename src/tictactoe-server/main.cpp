#include "server/game.hpp"
#include "server/eventsystem.hpp"
#include "server/communicationsystem.hpp"

int main( )
{
	game::event::setup( );
	game::networking::setup( );

	Sleep( INFINITE );
}