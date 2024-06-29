#pragma once

#include <windows.h>
#include <iostream>

namespace game
{
	enum interaction_type : unsigned char
	{
		move_up,
		move_down,
		move_left,
		move_right,
		click
	};

	enum pool_cage : unsigned char
	{
		free		 = 0,
		x			 = ( 1 << 0 ),
		o			 = ( 1 << 1 )
	};

	class pool_t
	{
	private:
		pool_cage pool_buffer[ 3 * 3 ];
		int selected_idx;

	public:
		pool_t( ) : selected_idx( 0 )
		{
			__stosb( reinterpret_cast< PBYTE >( this->pool_buffer ), 0, sizeof this->pool_buffer );
		}

		inline pool_cage *get( void )
		{
			return this->pool_buffer;
		}
		inline int &selected( )
		{
			return this->selected_idx;
		}
		template < class type_t > inline type_t get_as( void ) const
		{
			return type_t( this->pool_buffer );
		}
		template < class type_t > inline operator type_t( void )
		{
			return this->get_as< type_t >( );
		}
	};

	void exit( );

	extern pool_t *game_pool;
	extern bool is_in_game;
	extern bool connecting;
	extern bool your_turn;
	extern pool_cage team;
}