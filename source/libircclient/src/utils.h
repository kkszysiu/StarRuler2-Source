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
#ifndef INCLUDE_IRC_UTILS_H
#define INCLUDE_IRC_UTILS_H

 #include "session.h"

void libirc_add_to_set (int fd, fd_set *set, int * maxfd);

#if defined (ENABLE_DEBUG)
void libirc_dump_data (const char * prefix, const char * buf, unsigned int length);
#endif


/*
 * Finds a separator (\x0D\x0A), which separates two lines.
 */
int libirc_findcrlf (const char * buf, int length);
int libirc_findcrlf_offset(const char *buf, int offset, const int length);
int libirc_findcrorlf (char * buf, int length);

void libirc_event_ctcp_internal (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

#endif /* INCLUDE_IRC_UTILS_H */
