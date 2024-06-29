#include "../ahxrwinsock/ahxrwinsock.h"

#include "communicationsystem.hpp"

void *game::networking::_client = ( void * )( new AHXRCLIENT );

void game::networking::setup( const char *endpoint )
{
	bool connected = client->init( endpoint, "1299", TCP_SERVER, game::networking::on_client_connect );
	if ( !connected )
	{
		game::logging::push_log_entry( game::render::build_color( 0, 0xC ), "[ SEVERE ] Connection failure %s:%d", endpoint, 1299 );
		game::networking::terminate( );
		game::connecting = false;
		return;
	}

	game::logging::push_log_entry( game::render::build_color( 0, 0x7 ), "[ PROCESS ] Established connection with %s:%d", endpoint, 1299 );

	client->toggle_send_callback( true );
	client->set_toggle_callback( on_send_data );
	client->listen( on_recieve_data );
}

void game::networking::terminate( )
{
	client->close( );
}

void game::networking::on_client_connect( )
{
	return;
}

void game::networking::on_send_data( bool result, const char *content )
{
	struct
	{
		game::networking::error_code error_code;
		game::networking::packet_t packet;
	} network_context = { };

	if ( !result )
	{
		network_context.error_code = send_failed;
		game::event::container[ GAME_COMMUNICATOR ]->on_trigger( &network_context );
	}
}

void game::networking::on_recieve_data( char *content )
{
	struct
	{
		game::networking::error_code error_code;
		game::networking::packet_t packet;
	} network_context = { };

	network_context.error_code = success;
	network_context.packet = content;

	game::event::container[ GAME_COMMUNICATOR ]->on_trigger( &network_context );
}

void game::networking::send( packet_t packet )
{
	packet.rolling_xor( );
	client->send_data( packet.get_as< char * >( ), sizeof( packet_t::raw_packet_t ) );
}