/* 
 * Copyright (C) 2004-2012 George Yunaev gyunaev@ulduzsoft.com
 *
 * This library is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as published by 
 * the Free Software Foundation; either version 3 of the License, or (at your 
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public 
 * License for more details.
 */
#ifndef INCLUDE_IRC_SOCKETS_H
#define INCLUDE_IRC_SOCKETS_H

/*
 * The sockets interface was moved out to simplify going OpenSSL integration.
 */
#if !defined (_WIN32)
	#include <sys/socket.h>
	#include <netdb.h>
	#include <arpa/inet.h>	
	#include <netinet/in.h>
	#include <fcntl.h>
	#include <errno.h>

	#define IS_SOCKET_ERROR(a)	((a)<0)
	typedef int				socket_t;

#else
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <windows.h>

	#define IS_SOCKET_ERROR(a)	((a)==SOCKET_ERROR)

#if !defined(EWOULDBLOCK)
	#define EWOULDBLOCK		WSAEWOULDBLOCK
#endif
#if !defined(EINPROGRESS)
	#define EINPROGRESS		WSAEINPROGRESS
#endif
#if !defined(EINTR)
	#define EINTR			WSAEINTR
#endif
#if !defined(EAGAIN)
	#define EAGAIN			EWOULDBLOCK
#endif

	typedef SOCKET			socket_t;

#endif

#ifndef INADDR_NONE
	#define INADDR_NONE 	0xFFFFFFFF
#endif


int socket_error();

int socket_create (int domain, int type, socket_t * sock);
int socket_make_nonblocking (socket_t * sock);
int socket_close (socket_t * sock);
int socket_connect (socket_t * sock, const struct sockaddr *saddr, socklen_t len);
int socket_accept (socket_t * sock, socket_t * newsock, struct sockaddr *saddr, socklen_t * len);
int socket_recv (socket_t * sock, void * buf, size_t len);
int socket_send (socket_t * sock, const void *buf, size_t len);

#endif /* INCLUDE_IRC_SOCKETS_H */