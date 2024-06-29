#pragma once

#include <windows.h>
#include <vector>

namespace game::logging
{
	struct log_entry
	{
		unsigned short color;
		const char *text;
	};

	extern std::vector< log_entry > log_list;

	void push_log_entry( log_entry content );
	void push_log_entry( unsigned short color, const char *format, ... );
	void pop_log_entry( void );
	void clear( void );
}