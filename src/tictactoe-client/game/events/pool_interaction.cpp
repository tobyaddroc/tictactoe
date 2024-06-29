#include "pool_interaction.hpp"
#include "../game.hpp"

#include "../inputsystem.hpp"
#include "../logsystem.hpp"
#include "../rendersystem.hpp"
#include "../communicationsystem.hpp"

void pool_interaction::on_trigger( void *context )
{
	game::interaction_type interaction_type = *reinterpret_cast< decltype( interaction_type ) * >( context );

	switch ( interaction_type )
	{
		case game::move_up: 
		{
			if ( game::game_pool->selected( ) - 3 < 0 ) 
			{
				break;
			}
			game::game_pool->selected( ) -= 3;
			break;
		}
		case game::move_down: 
		{
			if ( game::game_pool->selected( ) + 3 > 8 ) 
			{
				break;
			}
			game::game_pool->selected( ) += 3;
			break;
		}
		case game::move_left: 
		{
			if ( game::game_pool->selected( ) - 1 < 0 )
			{
				break;
			}
			game::game_pool->selected( ) -= 1;
			break;
		}
		case game::move_right: 
		{
			if ( game::game_pool->selected( ) + 1 > 8 ) 
			{
				break;
			}
			game::game_pool->selected( ) += 1;
			break;
		}
		case game::click: 
		{
			if ( !game::is_in_game && !game::connecting ) 
			{
				game::connecting = true;
				game::networking::setup( game::input::virtual_stdin.data( ) );
				break;
			}
			else
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

				context.logic_type = game::networking::packet_type::make_turn;
				context.interaction.team = game::team;
				context.interaction.at = game::game_pool->selected( );

				game::event::container[ GAME_LOGIC ]->on_trigger( &context );
			}
			break;
		}
		default: 
		{
			break;
		}
	}
}