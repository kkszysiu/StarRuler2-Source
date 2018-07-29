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

#ifndef INCLUDE_IRC_COLORS_H
#define INCLUDE_IRC_COLORS_H

#include <ctype.h>

#define LIBIRC_COLORPARSER_BOLD			(1<<1)
#define LIBIRC_COLORPARSER_UNDERLINE	(1<<2)
#define LIBIRC_COLORPARSER_REVERSE		(1<<3)
#define LIBIRC_COLORPARSER_COLOR		(1<<4)

#define LIBIRC_COLORPARSER_MAXCOLORS	15

void libirc_colorparser_addorcat (char ** destline, unsigned int * destlen, const char * str);
void libirc_colorparser_applymask (unsigned int * mask, 
		char ** destline, unsigned int * destlen,
		unsigned int bitmask, const char * start, const char * end);
void libirc_colorparser_applycolor (unsigned int * mask, 
		char ** destline, unsigned int * destlen,
		unsigned int colorid, unsigned int bgcolorid);
void libirc_colorparser_closetags (unsigned int * mask, 
		char ** destline, unsigned int * destlen);



/*
 * IRC to [code] color conversion. Or strip.
 */
char * libirc_colorparser_irc2code (const char * source, int strip);
int libirc_colorparser_colorlookup (const char * color);

/*
 * [code] to IRC color conversion.
 */
char * irc_color_convert_to_mirc (const char * source);


char * irc_color_strip_from_mirc (const char * message);


char * irc_color_convert_from_mirc (const char * message);

#endif /* INCLUDE_IRC_COLORS_H */
