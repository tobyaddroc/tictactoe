#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "../ahxrwinsock/ahxrwinsock.h"

#include "communicationsystem.hpp"
#include "eventsystem.hpp"

void *game::networking::_server = ( void * )( new AHXRSERVER );
std::vector< SOCKET > game::networking::user_list = { };

#pragma warning( disable: 4477 )
#pragma warning( disable: 4313 )

void game::networking::setup( )
{
	bool started = server->start_server( "1299", TCP_SERVER, on_client_connect, on_recieve_data, on_client_disconnect );
	if ( !started )
	{
		std::cout << "Unable to start server" << std::endl;
		return;
	}

	std::cout << "Listening on 1299..." << std::endl;
}

void game::networking::terminate( )
{
	server->close( );
}

void game::networking::on_client_connect( SOCKET client_socket, void *address_info, SOCKADDR_IN *sock_addr )
{
	if ( user_list.size( ) > 1 )
	{
		printf( "Denied connection from socket 0x%X. Server is fully\n", client_socket );
		game::networking::send( client_socket, new packet_t::raw_packet_t( game::networking::packet_type::server_is_fully ) );
		WSABUF *wsa_buf = new WSABUF;
		WSARecvDisconnect( client_socket, wsa_buf );
		WSASendDisconnect( client_socket, wsa_buf );
		closesocket( client_socket );
		delete wsa_buf;
		return;
	}

	user_list.push_back( client_socket );

	game::networking::send( client_socket, new packet_t::raw_packet_t( game::networking::packet_type::connected ) );

	if ( user_list.size( ) == 2 )
	{
		game::networking::send( user_list[ 0 ], new packet_t::raw_packet_t( game::networking::packet_type::switch_turn ) );

		for ( auto &socket : user_list ) game::networking::send( socket, new packet_t::raw_packet_t( game::networking::packet_type::started ) );
		game::game_started = true;

		game::networking::send( user_list[ 0 ], new packet_t::raw_packet_t( game::networking::packet_type::teams, game::pool_cage::x ) );
		game::networking::send( user_list[ 1 ], new packet_t::raw_packet_t( game::networking::packet_type::teams, game::pool_cage::o ) );
	}

	printf( "Player joined the game. Players total: %d, Socket: 0x%X\n", user_list.size( ), client_socket );
}

void game::networking::on_client_disconnect( SOCKET client_socket, void *address_info, SOCKADDR_IN *sock_addr )
{
	if ( game::game_started )
	{
		if ( get_id( client_socket ) == -1 ) return;
		game::networking::send( user_list[ get_id( client_socket ) == 0 ? 1 : 0 ], new packet_t::raw_packet_t( game::networking::packet_type::game_over_win ) );
		game::game_started = false;

		__stosb( game::game_pool->get_as< PBYTE >( ), game::pool_cage::free, 9 );
	}

	if ( get_id( client_socket ) != -1 )
	{
		user_list.erase( user_list.begin( ) + get_id( client_socket ) );
		printf( "Player left the game. Players total: %d, Socket: 0x%X\n", user_list.size( ), client_socket );
	}
}

int game::networking::get_id( SOCKET client_socket )
{
	for ( auto j = 0; j < user_list.size( ); j++ )
	{
		if ( client_socket == user_list[ j ] )
		{
			return j;
		}
	}
	return -1;
}

void game::networking::on_recieve_data( SOCKET client_socket, void *address_info, SOCKADDR_IN *sock_addr, char *data )
{
	packet_t packet = data;
	packet.rolling_xor( );

	struct
	{
		int id;
		packet_t::raw_packet_t *packet;
	} context = { get_id( client_socket ), packet.get_as< packet_t::raw_packet_t * >( ) };

	printf( "Recieved %d bytes from 0x%X socket. Interaction type: %d, Data: %p\n", sizeof( game::networking::packet_t::raw_packet_t ), client_socket, packet.get_as< game::networking::packet_t::raw_packet_t * >( )->type, packet.get_as< game::networking::packet_t::raw_packet_t * >( )->data );

	game::event::container[ NETWORK_DISPATCHER ]->on_trigger( &context );
}

void game::networking::send( SOCKET client_socket, packet_t packet )
{
	Sleep( 100 );

	printf( "Sending %d bytes to 0x%X socket. Interaction type: %d, Data: %p\n", sizeof( game::networking::packet_t::raw_packet_t ), client_socket, packet.get_as< game::networking::packet_t::raw_packet_t * >( )->type, packet.get_as< game::networking::packet_t::raw_packet_t * >( )->data );

	packet.rolling_xor( );
	server->send_data( client_socket, packet.get_as< char * >( ), sizeof( game::networking::packet_t::raw_packet_t ) );
}

void game::networking::eliminate( SOCKET client_socket )
{
	printf( "Socket 0x%X eliminated. Cheating/Exploiting.\n", client_socket );
	game::networking::send( client_socket, new packet_t::raw_packet_t( game::networking::packet_type::disconnected_cheating ) );
	WSABUF *wsa_buf = new WSABUF;
	WSARecvDisconnect( client_socket, wsa_buf );
	WSASendDisconnect( client_socket, wsa_buf );
	closesocket( client_socket );
	delete wsa_buf;
}