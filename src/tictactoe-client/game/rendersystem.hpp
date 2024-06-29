#pragma once

#include "game.hpp"

namespace game::render
{
	extern HANDLE console_handle;
	extern HWND console_wnd;
	extern CHAR hot_scene[ 120 * 30 + 1 ];
	extern CHAR dummy_scene[ 120 * 30 + 1 ];
	extern unsigned __int64 frame_count;

	void setup( );

	void draw( );
	void reset( );

	void put( COORD coordinates, const char *format, DWORD size, ... );
	void put( DWORD coordinates, const char *format, DWORD size, ... );
	void put( COORD coordinates, const char *source );
	void put( DWORD coordinates, const char *source );
	void put( COORD coordinates, char source );
	void put( DWORD coordinates, char source );

	void paint( );
	void paint( COORD coordinates, WORD color, SIZE_T size );

	inline extern void delay( DWORD milliseconds );

	inline extern WORD build_color( WORD background, WORD foreground );
}