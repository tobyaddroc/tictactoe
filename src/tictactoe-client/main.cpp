#include <windows.h>

#include "game/game.hpp"
#include "game/inputsystem.hpp"
#include "game/logsystem.hpp"
#include "game/rendersystem.hpp"
#include "game/eventsystem.hpp"

#define render_cage( idx ) pool[ idx ] == game::pool_cage::x ? "x" : pool[ idx ] == game::pool_cage::o ? "o" : "."
#define color_cage( idx ) pool[ idx ] == game::pool_cage::x ? 0xC : pool[ idx ] == game::pool_cage::o ? 0xB : 0x8

int main( )
{
	short entry_counter = 0;
	bool fixed_color_correction = false;

	game::event::setup( );
	game::input::setup( );
	game::render::setup( );

	for ( ;; )
	{
		game::render::reset( );
		game::input::run( );

		if ( !game::is_in_game )
		{
			game::render::put( { 0, 0 }, "Server IP: %s", 30, game::input::virtual_stdin.data( ) );
			game::render::put( { 0, 2 }, "==========  LOG  ==========" );
			for ( int j = 0; j < game::logging::log_list.size( ); j++ )
			{
				game::logging::log_entry log_entry = game::logging::log_list[ j ];
				game::render::paint( { 0, short( 3 + entry_counter ) }, log_entry.color, strlen( log_entry.text ) );
				game::render::put( { 0, short( 3 + entry_counter ) }, log_entry.text );
				entry_counter++;
			}
			entry_counter ^= entry_counter;
		}
		else
		{
			game::render::put( { 0, 0 }, game::your_turn ? "Your turn" : "Waiting for your enemy..." );
			game::render::put( { 30, 0 }, "Your team: %s", 15, game::team == game::pool_cage::x ? "X" : game::team == game::pool_cage::o ? "O" : "???" );

			auto pool = game::game_pool->get( );

			if ( !fixed_color_correction )
			{
				fixed_color_correction = true;
				game::render::paint( { 0, 2 }, game::render::build_color( 0, 0x7 ), 15 );
				game::render::paint( { 0, 3 }, game::render::build_color( 0, 0x7 ), 15 );
				game::render::paint( { 0, 4 }, game::render::build_color( 0, 0x7 ), 15 );
				game::render::paint( { 0, 5 }, game::render::build_color( 0, 0x7 ), 15 );
				game::render::paint( { 0, 6 }, game::render::build_color( 0, 0x7 ), 15 );
			}

			game::render::put( { 3, 2 }, "%s %s %s", 10, render_cage( 0 ), render_cage( 1 ), render_cage( 2 ) );
			game::render::put( { 3, 3 }, "%s %s %s", 10, render_cage( 3 ), render_cage( 4 ), render_cage( 5 ) );
			game::render::put( { 3, 4 }, "%s %s %s", 10, render_cage( 6 ), render_cage( 7 ), render_cage( 8 ) );

			for ( auto idx = 0; idx < 9; idx++ ) {
				game::render::paint( { short( 3 + idx % 3 * 2 ), short( 2 + idx / 3 ) }, game::render::build_color( 0, color_cage( idx ) ), 1 );
			}

			auto selected = game::game_pool->selected( );

			game::render::paint( { short( 3 + selected % 3 * 2 ), short( 2 + selected / 3 ) }, game::render::build_color( 0, 0xF ), 1 );

			game::render::put( { 0, 6 }, "==========  LOG  ==========" );
			for ( int j = 0; j < game::logging::log_list.size( ); j++ )
			{
				game::logging::log_entry log_entry = game::logging::log_list[ j ];
				game::render::paint( { 0, short( 7 + entry_counter ) }, log_entry.color, strlen( log_entry.text ) );
				game::render::put( { 0, short( 7 + entry_counter ) }, log_entry.text );
				entry_counter++;
			}
			entry_counter ^= entry_counter;
		}

		game::render::draw( );
		game::render::frame_count++;

		game::render::delay( 1 );
	}
}