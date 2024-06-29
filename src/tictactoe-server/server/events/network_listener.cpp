#include "network_listener.hpp"

#include "../communicationsystem.hpp"
#include "../game.hpp"

#pragma warning( disable: 4477 )

void network_dispatcher::on_trigger( void *context )
{
	struct
	{
		int id;
		game::networking::packet_t::raw_packet_t *packet;
	} *network_context = reinterpret_cast< decltype( network_context ) >( context );

	switch ( network_context->packet->type )
	{
		case game::networking::packet_type::make_turn:
		{
			auto target = game::networking::user_list[ network_context->id == 0 ? 1 : 0 ];

			if ( !game::game_started )
			{
				printf( "Illegal turn recieved from 0x%X. Game not started\n", game::networking::user_list[ network_context->id ] );
				game::networking::eliminate( game::networking::user_list[ network_context->id ] );
				break;
			}
			if ( ( network_context->id == 0 && game::turn == game::o ) || ( network_context->id == 1 && game::turn == game::x ) )
			{
				printf( "Illegal turn recieved from 0x%X. Team %d must make turn.\n", game::networking::user_list[ network_context->id ], game::turn );
				game::networking::eliminate( game::networking::user_list[ network_context->id ] );
				break;
			}
			if ( network_context->packet->data < 0 || network_context->packet->data > 8 )
			{
				printf( "Illegal turn recieved from 0x%X. Index out of bounds.\n", game::networking::user_list[ network_context->id ] );
				game::networking::eliminate( game::networking::user_list[ network_context->id ] );
				break;
			}
			if ( game::game_pool->get( )[ network_context->packet->data ] != game::pool_cage::free )
			{
				printf( "Illegal turn recieved from 0x%X. Cage is not free.\n", game::networking::user_list[ network_context->id ] );
				game::networking::eliminate( game::networking::user_list[ network_context->id ] );
				break;
			}

			game::game_pool->get( )[ network_context->packet->data ] = network_context->id == 0 ? game::x : game::o;
			game::turn = game::turn == game::x ? game::o : game::turn == game::o ? game::x : game::x;

			printf( "Socket 0x%X made a turn. Index: %d\n", game::networking::user_list[ network_context->id ], network_context->packet->data );

			game::networking::send( target, new game::networking::packet_t::raw_packet_t( game::networking::packet_type::update_pool, network_context->packet->data ) );
			game::networking::send( target, new game::networking::packet_t::raw_packet_t( game::networking::packet_type::switch_turn ) );

			if ( game::check_win( game::x ) )
			{
				printf( "Victory for socket 0x%X (%s)\n", game::networking::user_list[ network_context->id ], "X" );
				game::networking::send( game::networking::user_list[ 0 ], new game::networking::packet_t::raw_packet_t( game::networking::packet_type::game_over_win ) );
				game::networking::send( game::networking::user_list[ 1 ], new game::networking::packet_t::raw_packet_t( game::networking::packet_type::game_over_lose ) );
				
				__stosb( game::game_pool->get_as< PBYTE >( ), game::pool_cage::free, 9 );
				game::game_started = false;
			}
			if ( game::check_win( game::o ) )
			{
				printf( "Victory for socket 0x%X (%s)\n", game::networking::user_list[ network_context->id ], "O" );
				game::networking::send( game::networking::user_list[ 1 ], new game::networking::packet_t::raw_packet_t( game::networking::packet_type::game_over_win ) );
				game::networking::send( game::networking::user_list[ 0 ], new game::networking::packet_t::raw_packet_t( game::networking::packet_type::game_over_lose ) );
				
				__stosb( game::game_pool->get_as< PBYTE >( ), game::pool_cage::free, 9 );
				game::game_started = false;
			}

			break;
		}
	}
}