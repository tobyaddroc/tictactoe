#include "rendersystem.hpp"

#include <cassert>

HANDLE game::render::console_handle = nullptr;
CHAR game::render::hot_scene[ 120 * 30 + 1 ] = { };
CHAR game::render::dummy_scene[ 120 * 30 + 1 ] = { };
unsigned __int64 game::render::frame_count = 0;

void game::render::setup( )
{
	COORD coordinates = { 120, 30 };
	SMALL_RECT rectangle = { 0, 0, 29, 119 };
	HANDLE handle = GetStdHandle( STD_OUTPUT_HANDLE );
	HWND console_wnd = GetConsoleWindow( );
	LONG console_style = GetWindowLongA( console_wnd, GWL_STYLE );
	SetConsoleScreenBufferSize( handle, coordinates );
	SetConsoleWindowInfo( handle, 1, &rectangle );
	console_handle = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0 );
	SetConsoleActiveScreenBuffer( console_handle );
	SetWindowLongA( console_wnd, GWL_STYLE, console_style & ~( WS_THICKFRAME | WS_MAXIMIZEBOX ) );
	SetConsoleTitleA( "TicTacToe" );
}

void game::render::paint( COORD coordinates, WORD color, SIZE_T size )
{
	for ( auto idx = 0; idx < size; idx++ )
	{
		DWORD bytes_painted = 0;
		WriteConsoleOutputAttribute( console_handle, &color, 1, coordinates, &bytes_painted );
		coordinates.X++;
	}
}
void game::render::paint( )
{
	for ( short pixel = 0; pixel < sizeof hot_scene; pixel++ )
	{
		game::render::paint( { ( short )( pixel % 120 ), ( short )( trunc( pixel / 120 ) ) }, game::render::build_color( 0, 7 ), 1 );
	}
}

void game::render::put( COORD coordinates, const char *format, DWORD size, ... )
{
	assert( format );
	assert( size > 0 );

	va_list va_arguments;
	va_start( va_arguments, format );

	char *buffer = new char[ size ];
	wvsprintfA( buffer, format, va_arguments );

	game::render::put( coordinates, buffer );

	delete[ ] buffer;
}
void game::render::put( DWORD coordinates, const char *format, DWORD size, ... )
{
	assert( format );
	assert( size > 0 );

	va_list va_arguments;
	va_start( va_arguments, format );

	char *buffer = new char[ size ];
	wvsprintfA( buffer, format, va_arguments );

	game::render::put( coordinates, buffer );

	delete[ ] buffer;
}
void game::render::put( COORD coordinates, const char *source )
{
	assert( source );

	size_t sizeof_buffer = strlen( source ) + 1;
	assert( sizeof_buffer > 0 );

	strcpy_s( &hot_scene[ coordinates.X + coordinates.Y * 120 ], sizeof_buffer, source );
}
void game::render::put( DWORD coordinates, const char *source )
{
	assert( source );

	size_t sizeof_buffer = strlen( source ) + 1;
	assert( sizeof_buffer > 0 );

	strcpy_s( &hot_scene[ coordinates ], sizeof_buffer, source );
}
void game::render::put( COORD coordinates, char source )
{
	assert( source );

	__stosb( ( PBYTE )&hot_scene[ coordinates.X + coordinates.Y * 120 ], source, 1 );
}
void game::render::put( DWORD coordinates, char source )
{
	assert( source );

	__stosb( ( PBYTE )&hot_scene[ coordinates ], source, 1 );
}

void game::render::draw( )
{
	DWORD bytes_written = 0;
	WriteConsoleOutputCharacterA( console_handle, hot_scene, sizeof hot_scene, { 0, 0 }, &bytes_written );
}
void game::render::reset( )
{
	__stosb( PBYTE( hot_scene ), 0, sizeof hot_scene );
}

void game::render::delay( DWORD milliseconds )
{
	return Sleep( milliseconds );
}

WORD game::render::build_color( WORD background, WORD foreground )
{
	return ( ( ( background << 4 ) | foreground ) );
}