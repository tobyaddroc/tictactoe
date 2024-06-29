#include "logsystem.hpp"

std::vector< game::logging::log_entry > game::logging::log_list = { };

void game::logging::push_log_entry( log_entry content )
{
	size_t sizeof_format = strlen( content.text ) + 1;
	char *buffer = new char[ sizeof_format ];
	__movsb( PBYTE( buffer ), PBYTE( content.text ), sizeof_format );

	content.text = buffer;

	log_list.push_back( content );
}

void game::logging::push_log_entry( unsigned short color, const char *format, ... )
{
	va_list va_args;
	__crt_va_start( va_args, format );

	char *buffer = new char[ 0x100 ];
	wvsprintfA( buffer, format, va_args );

	log_list.push_back( { color, buffer } );
}

void game::logging::pop_log_entry( void )
{
	log_list.pop_back( );
}

void game::logging::clear( void )
{
	for ( auto &log_entry : log_list ) delete[ ] log_entry.text;
	log_list.clear( );
}