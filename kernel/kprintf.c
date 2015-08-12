/*    Loki 0.01 operating system
 *    Copyright (C) 2014 Wojciech Ró¿owski
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 *    USA
 */

#include <loki/system.h>

void kprintf (const char *format, ...)
{
	uint8_t **arg = (uint8_t **) &format;
	int c;
	uint8_t buf[20];

	arg++;

	while ((c = *format++) != 0)
	{
		if (c != '%')
		{
			putch (c);
		}
		else
		{
			uint8_t *p;

			c = *format++;
			switch (c)
			{
				case 'd':
				case 'u':
				case 'x':
					itoa (buf, c, *((int *) arg++));
					p = buf;
					goto string;
				break;
				case 's':
					p = *arg++;
					if (! p)
					{
						p ='\0';
					}
				string:
					while (*p)
					{
						putch (*p++);
					}
				break;

				default:
					putch (*((int *) arg++));
					break;
			}
		}
	}
}
void printklog (const char *format, ...)
{
	uint8_t **arg = (uint8_t **) &format;
	int c;
	uint8_t buf[20];

	arg++;

	while ((c = *format++) != 0)
	{
		if (c != '%')
		{
			write_serial (c);
		}
		else
		{
			uint8_t *p;

			c = *format++;
			switch (c)
			{
				case 'd':
				case 'u':
				case 'x':
					itoa (buf, c, *((int *) arg++));
					p = buf;
					goto string;
				break;
				case 's':
					p = *arg++;
					if (! p)
					{
						p ='\0';
					}
				string:
					while (*p)
					{
						write_serial (*p++);
					}
				break;

				default:
					write_serial (*((int *) arg++));
					break;
			}
		}
	}
}
