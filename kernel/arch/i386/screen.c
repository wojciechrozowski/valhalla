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

uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

uint16_t make_vgaentry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

size_t strlen(const char* str)
{
	size_t ret = 0;
	while ( str[ret] != 0 )
		ret++;
	return ret;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminalX;
size_t terminalY;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize()
{
	terminalX = 0;
	terminalY = 0;
	move_cursor();
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}
void move_cursor()
{
   uint16_t cursorLocation = terminalX * 80 + terminalY;
   outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
   outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
   outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
   outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}
void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

void putch(char c)
{
	if (terminalY>=80)
	{
		terminalY=0;
		terminalX++;
	}

	switch(c)
	{
		case '\n':
			terminalY = 0;
			terminalX++;
			break;
		case '\r':
			terminalY=0;
		    break;
		case '\t':
			terminalY = (terminalY + 8) & ~(8-1);
			break;
		case 0x08:
			terminalY--;
			break;
		default:

	terminal_putentryat(c, terminal_color, terminalY, terminalX);
	if ( ++terminalY == VGA_WIDTH )
	{
		terminalY = 0;
		if ( ++terminalX == VGA_HEIGHT )
		{
			terminalX = 0;

		}
	}

	}
	scroll();
	move_cursor();
}

void puts(const char* data)
{
	size_t datalen = strlen(data);
	for ( size_t i = 0; i < datalen; i++ )
		putch(data[i]);
}

void scroll()
{
	if (terminalX >= 25)
	{
		int i;
		for(i=0*25;i<24*80;i++)
		{
			terminal_buffer[i] = terminal_buffer[i+80];
		}
		for(i=80*24;i<80*25;i++)
		{
			terminal_buffer[i] = (0xF<<8) | ' ';
		}
		terminalX=24;
	}

}


