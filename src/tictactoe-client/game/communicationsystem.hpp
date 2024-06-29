#pragma once

#include "game.hpp"
#include "inputsystem.hpp"
#include "logsystem.hpp"
#include "rendersystem.hpp"
#include "eventsystem.hpp"

#define client reinterpret_cast< LPAHXRCLIENT >( _client )

namespace game::networking
{
	extern void *_client;

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

			raw_packet_t( packet_type packet = game::networking::packet_type::make_turn, int data = 0 ) : type( packet ), data( data )
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

	void setup( const char *end_point );
	void terminate( );

	void on_client_connect( );
	void on_send_data( bool result, const char *content );
	void on_recieve_data( char *content );

	void send( packet_t packet );
}