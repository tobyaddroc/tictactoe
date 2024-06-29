/*
	@title
		ahxrwinsock
	@author
		AHXR (https://github.com/AHXR)
	@copyright
		2017

	ahxrwinsock is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	ahxrwinsock is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with ahxrwinsock.  If not, see <http://www.gnu.org/licenses/>.
*/
//=======================================================
#pragma once

#ifndef AHXRWINSOCK_WRAPPER
#define AHXRWINSOCK_WRAPPER

#define					WIN32_LEAN_AND_MEAN

#include				<winsock2.h>
#include				<WS2tcpip.h>
#include				<iostream>
#include				<string>
#include				<Windows.h>
#include				<stdlib.h>
#include				<stdio.h>
#include				<vector>

#ifdef AHXRLOGGER_PLUGIN
	#include				"ahxrlogger.h"
#endif

typedef void( *FPCLIENT_CB ) ( );
typedef void( *FPC_CALLBACK ) ( SOCKET clientSocket, void *addressInfo, SOCKADDR_IN *addrInfo );
typedef void( *FPS_CALLBACK ) ( SOCKET clientSocket, void *addressInfo, SOCKADDR_IN *addrInfo, char *data );
typedef void( *FPD_CALLBACK )( SOCKET clientSocket, void *addressInfo, SOCKADDR_IN *addrInfo );
typedef void( *FP_RES ) ( char *recData );
typedef void( *FPCLIENT_T ) ( bool result, const char *dataSent );

typedef addrinfo *CLIENTDATA;

#define					AHXRSOCKET_PREFIX				"AHXRSOCKET:"
#define					AHXRSOCKET_CLIENT_PREFIX		"AHXRSOCKET CLIENT:"

#ifndef DEFAULT_BUFF
#define				DEFAULT_BUFF					512
#endif

#define					AHXRCLIENT						__AHXRSOCKET__::Client
#define					LPAHXRCLIENT					__AHXRSOCKET__::Client *
#define					AHXRSERVER						__AHXRSOCKET__::Server
#define					LPAHXRSERVER					__AHXRSOCKET__::Server *

#define					TCP_SERVER						IPPROTO_TCP
#define					IP_SERVER						IPPROTO_IP
#define					IPV6_SERVER						IPPROTO_IPV6
#define					RM_SERVER						IPPROTO_RM
#define					UDP_SERVER						IPPROTO_UDP

#pragma comment			( lib,   "Ws2_32.lib" )

#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif


namespace __AHXRSOCKET__ {
	class Client 
	{
		private:
			FPCLIENT_CB		Client_Callback;
			FPCLIENT_T		Toggle_Callback;
			bool			b_toggle;
			bool			b_sd;

		public: 
			SOCKET			Socket_Client;
			bool			init					( PCSTR server, PCSTR port, int protocol, FPCLIENT_CB fpCB );
			bool			send_data				( const char * sendChar, int len );
			void			set_toggle_callback		( FPCLIENT_T pointerFunc );
			void			toggle_send_callback	( bool toggle );
			void			listen					( FP_RES fpRes, bool thread = true );
			void			__listenfunc			( FP_RES fpRes );
			void			close					();
			FP_RES			fp_params;

	};

	class Server 
	{
		public: 
			SOCKET				Socket_Listen;
			SOCKET				Socket_Client;

		public:
			bool				start_server		(PCSTR port, int protocol, FPC_CALLBACK fpConnected=0, FPS_CALLBACK fpData=0, FPD_CALLBACK fpDisconnected=0, bool thread = true);
			void				send_data			(SOCKET clientSocket, const char * sendChar, int len);
			void				close				();
	};

	DWORD WINAPI			__t__listenThread(LPVOID lpParams);
	DWORD WINAPI			__t__startserver(LPVOID lpParams);
	DWORD WINAPI			__t__createServerRes(LPVOID lpParams);
	bool					__createserver(AHXRSERVER * const & server_class, PCSTR port, int protocol, FPC_CALLBACK fpConnected, FPS_CALLBACK fpData, FPD_CALLBACK fpDisconnected );
	void					cleanWSA(PADDRINFOA pAddr, SOCKET s = INVALID_SOCKET);

	struct s_server_params
	{
		PCSTR			port;
		int				protocol;
		FPC_CALLBACK	fpConnected;
		FPS_CALLBACK	fpData;
		FPD_CALLBACK	fpDisconnected;
		SOCKET			socket_l;
		SOCKET			socket_c;
		AHXRSERVER *	s_class;
	};

	struct s_server_res {
		SOCKET SocketResponse;
		FPS_CALLBACK fpResData;
		FPD_CALLBACK fpDisconnected;
		struct addrinfo * addrRes;
		struct sockaddr_in * sockAddr;
	};
}

#undef _CRT_SECURE_NO_WARNINGS
#endif