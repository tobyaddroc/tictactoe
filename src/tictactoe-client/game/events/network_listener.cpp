#include "network_listener.hpp"

#include "../logsystem.hpp"
#include "../rendersystem.hpp"
#include "../communicationsystem.hpp"

#include "../game.hpp"

void network_dispatcher::on_trigger( void *context )
{
	struct
	{
		game::networking::error_code error_code;
		game::networking::packet_t packet;
	} *network_context = reinterpret_cast< decltype( network_context ) >( context );

	if ( network_context->error_code == game::networking::error_code::send_failed )
	{
		game::is_in_game = false;
		game::networking::terminate( );

		MessageBoxA( GetConsoleWindow( ), "Disconnected due to internal error", "TicTacToe", MB_OK | MB_ICONERROR );

		game::exit( );
		return;
	}

	network_context->packet.rolling_xor( );

	auto dissected_packet = network_context->packet.get_as< game::networking::packet_t::raw_packet_t * >( );
	switch ( dissected_packet->type )
	{
		case game::networking::packet_type::connected: 
		{
			game::logging::push_log_entry( game::render::build_color( 0, 0xA ), "[ SUCCESS ] Handshake completed" );
			game::logging::push_log_entry( game::render::build_color( 0, 0x7 ), "[ PROCESS ] Waiting for your opponent..." );
			break;
		}
		case game::networking::packet_type::started:
		{
			game::is_in_game = true;
			game::logging::push_log_entry( game::render::build_color( 0, 0xA ), "[ SUCCESS ] Game started" );
			break;
		}
		case game::networking::packet_type::switch_turn:
		{
			game::your_turn = !game::your_turn;
			break;
		}
		case game::networking::packet_type::teams:
		{
			game::team = game::pool_cage( dissected_packet->data );
			break;
		}
		case game::networking::packet_type::update_pool:
		{
			struct
			{
				game::networking::packet_type logic_type;
				struct
				{
					game::pool_cage team;
					int at;
				} interaction;
			} context = { };

			context.logic_type = game::networking::packet_type::update_pool;
			context.interaction.team = game::team == game::x ? game::o : game::team == game::o ? game::x : game::x;
			context.interaction.at = dissected_packet->data;

			game::event::container[ GAME_LOGIC ]->on_trigger( &context );
			break;
		}
		case game::networking::packet_type::game_over_win:
		{
			struct
			{
				game::networking::packet_type logic_type;
				struct
				{
					game::pool_cage team;
					int at;
				} interaction;
			} context = { };

			context.logic_type = game::networking::packet_type::game_over_win;

			game::event::container[ GAME_LOGIC ]->on_trigger( &context );
			break;
		}
		case game::networking::packet_type::game_over_lose:
		{
			struct
			{
				game::networking::packet_type logic_type;
				struct
				{
					game::pool_cage team;
					int at;
				} interaction;
			} context = { };

			context.logic_type = game::networking::packet_type::game_over_lose;

			game::event::container[ GAME_LOGIC ]->on_trigger( &context );
			break;
		}
		case game::networking::packet_type::server_is_fully:
		{
			game::logging::push_log_entry( game::render::build_color( 0, 0xC ), "[ SEVERE ] Handshake with %s:%d failed: Server is fully.", game::input::virtual_stdin.data( ), 1299 );
			game::connecting = false;
			break;
		}
		case game::networking::packet_type::disconnected_cheating:
		{
			game::is_in_game = false;
			game::connecting = false;
			__stosb( game::game_pool->get_as< PBYTE >( ), game::pool_cage::free, 9 );
			game::logging::push_log_entry( game::render::build_color( 0, 0xC ), "[ SEVERE ] You have been eliminated due to high ping, cheating/exploiting or for other offenses." );
			MessageBoxA( GetConsoleWindow( ), "You have been eliminated due to high ping, cheating/exploiting or for other offenses.", "TicTacToe", MB_OK | MB_ICONERROR );
			break;
		}
		default: 
		{
			break;
		}
	}
}