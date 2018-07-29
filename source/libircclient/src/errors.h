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
#ifndef INCLUDE_IRC_ERROR_SESSION_H
#define INCLUDE_IRC_ERROR_SESSION_H

//#include "libircclient.h"
#include "session.h"

int irc_errno (irc_session_t * session);
const char * irc_strerror (int ircerrno);

#endif /* INCLUDE_IRC_ERROR_SESSION_H */
