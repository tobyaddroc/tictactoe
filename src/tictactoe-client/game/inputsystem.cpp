#include "inputsystem.hpp"

#include "rendersystem.hpp"

#include "eventsystem.hpp"
#include "events/pool_interaction.hpp"

#include "logsystem.hpp"

#include <string>

std::vector< char > game::input::virtual_stdin = { };

void game::input::setup( )
{
	game::input::virtual_stdin.reserve( 0x100 );
	__stosb( PBYTE( game::input::virtual_stdin.data( ) ), 0, 0x100 );
}

void game::input::run( )
{
	if ( GetConsoleWindow( ) == GetForegroundWindow( ) )
	{
		if ( GetAsyncKeyState( VK_UP ) & ( 1 << 0 ) ) game::event::container[ POOL_INTERACTION ]->on_trigger( ( void * )( new interaction_type( interaction_type::move_up ) ) );
		if ( GetAsyncKeyState( VK_DOWN ) & ( 1 << 0 ) ) game::event::container[ POOL_INTERACTION ]->on_trigger( ( void * )( new interaction_type( interaction_type::move_down ) ) );
		if ( GetAsyncKeyState( VK_LEFT ) & ( 1 << 0 ) ) game::event::container[ POOL_INTERACTION ]->on_trigger( ( void * )( new interaction_type( interaction_type::move_left ) ) );
		if ( GetAsyncKeyState( VK_RIGHT ) & ( 1 << 0 ) ) game::event::container[ POOL_INTERACTION ]->on_trigger( ( void * )( new interaction_type( interaction_type::move_right ) ) );
		if ( GetAsyncKeyState( VK_RETURN ) & ( 1 << 0 ) ) game::event::container[ POOL_INTERACTION ]->on_trigger( ( void * )( new interaction_type( interaction_type::click ) ) );
		if ( GetAsyncKeyState( VK_DELETE ) & ( 1 << 0 ) ) game::logging::clear( );
		
		for ( char nums = VK_NUMPAD0; nums <= VK_NUMPAD9; nums++ )
		{
			if ( ( GetAsyncKeyState( nums ) & ( 1 << 0 ) ) && game::input::virtual_stdin.size( ) < 0x10 )
			{
				game::input::virtual_stdin.push_back( nums - 0x60 + 48 );
			}
		}
		for ( char nums = '0'; nums <= '9'; nums++ )
		{
			if ( ( GetAsyncKeyState( nums ) & ( 1 << 0 ) ) && game::input::virtual_stdin.size( ) < 0x10 )
			{
				game::input::virtual_stdin.push_back( nums );
			}
		}
		if ( GetAsyncKeyState( VK_OEM_PERIOD ) & ( 1 << 0 ) ) game::input::virtual_stdin.push_back( '.' );

		if ( GetAsyncKeyState( VK_BACK ) & ( 1 << 0 ) && game::input::virtual_stdin.size( ) > 0 )
		{
			game::input::virtual_stdin[ game::input::virtual_stdin.size( ) - 1 ] = '\0';
			game::input::virtual_stdin.pop_back( );
		}
	}
}