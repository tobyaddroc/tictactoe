#pragma once

#include "game.hpp"

#define server reinterpret_cast< LPAHXRSERVER >( _server )

namespace game::networking
{
	extern void *_server;
	extern std::vector< SOCKET > user_list;

	enum error_code : unsigned char
	{
		success,
		send_failed
	};

	enum packet_type : unsigned char
	{
		update_pool,
		make_turn,
		game_over_win,
		game_over_lose,
		disconnected_cheating,
		server_is_fully,
		started,
		switch_turn,
		connected,
		teams
	};

	class packet_t
	{
	public:
		struct raw_packet_t
		{
			packet_type type;
			int data;

			raw_packet_t( packet_type packet, int data = 0 ) : type( packet ), data( data )
			{
				return;
			}
		};

		inline packet_t( raw_packet_t *packet ) : buffer( ( char * )( packet ) ) { }
		inline packet_t( char *packet ) : buffer( packet ) { }
		inline packet_t( void ) : buffer( nullptr ) { }

		inline void rolling_xor( void ) noexcept( true )
		{
			for ( unsigned idx = 0; idx < sizeof( raw_packet_t ); idx++ )
			{
				this->buffer[ idx ] ^= 0x6A;
			}
		}
		
		template < class type_t > inline type_t get_as( void ) noexcept( true )
		{
			return type_t( this->buffer );
		}

	private:
		char *buffer;
	};

	void setup( );
	void terminate( );

	void on_client_connect( SOCKET client_socket, void *address_info, SOCKADDR_IN *sock_addr );
	void on_client_disconnect( SOCKET client_socket, void *address_info, SOCKADDR_IN *sock_addr );
	void on_recieve_data( SOCKET client_socket, void *address_info, SOCKADDR_IN *sock_addr, char *data );
	int get_id( SOCKET client_socket );

	void send( SOCKET client_socket, packet_t packet );

	void eliminate( SOCKET client_socket );
}