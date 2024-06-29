#include "game_logic.hpp"

#include "../logsystem.hpp"
#include "../rendersystem.hpp"
#include "../communicationsystem.hpp"

#include "../game.hpp"

void game_logic::on_trigger( void *context )
{
	struct
	{
		game::networking::packet_type logic_type;
		struct
		{
			game::pool_cage team;
			int at;
		} interaction;
	} *logic_context = reinterpret_cast< decltype( logic_context ) >( context );

	switch ( logic_context->logic_type )
	{
		case game::networking::packet_type::update_pool:
		{
			if ( logic_context->interaction.team == game::pool_cage::free ) 
			{
				break;
			}
			if ( logic_context->interaction.at < 0 || logic_context->interaction.at > 8 )
			{
				break;
			}

			game::game_pool->get( )[ logic_context->interaction.at ] = logic_context->interaction.team;
			break;
		}
		case game::networking::packet_type::make_turn:
		{
			if ( !game::your_turn )
			{
				break;
			}
			if ( logic_context->interaction.team == game::pool_cage::free )
			{
				break;
			}
			if ( logic_context->interaction.at < 0 || logic_context->interaction.at > 8 )
			{
				break;
			}
			if ( game::game_pool->get( )[ logic_context->interaction.at ] != game::pool_cage::free )
			{
				break;
			}

			game::game_pool->get( )[ logic_context->interaction.at ] = logic_context->interaction.team;
			game::your_turn = !game::your_turn;

			auto packet = new game::networking::packet_t::raw_packet_t;
			packet->type = game::networking::packet_type::make_turn;
			packet->data = logic_context->interaction.at;

			game::networking::send( packet );
			break;
		}
		case game::networking::packet_type::game_over_win:
		{
			game::networking::terminate( );

			MessageBoxA( GetConsoleWindow( ), "Victory", "TicTacToe", MB_OK | MB_ICONINFORMATION );

			game::is_in_game = false;
			game::connecting = false;

			game::logging::clear( );
			__stosb( game::game_pool->get_as< PBYTE >( ), game::pool_cage::free, 9 );

			break;
		}
		case game::networking::packet_type::game_over_lose:
		{
			game::networking::terminate( );

			MessageBoxA( GetConsoleWindow( ), "Defeat", "TicTacToe", MB_OK | MB_ICONERROR );

			game::is_in_game = false;	
			game::connecting = false;

			game::logging::clear( );
			__stosb( game::game_pool->get_as< PBYTE >( ), game::pool_cage::free, 9 );

			break;
		}
	}
}