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

void memset(uint8_t *dest, uint8_t val, uint32_t len)
{
	uint8_t *temp = (uint8_t *)dest;
	for ( ; len != 0; len--) *temp++ = val;
}

void itoa (uint8_t *buf, uint32_t base, uint32_t d)
{
	uint8_t *p = buf;
	uint8_t *p1, *p2;
	uint32_t ud = d;
	uint32_t divisor = 10;


	if (base == 'd')
	{
		*p++ = '-';
		buf++;
		ud = -d;
	}
	else if (base == 'x')
	{
		divisor = 16;
	}

	do
	{
		uint32_t remainder = ud % divisor;

		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
	}
	while (ud /= divisor);

	*p = 0;
	p1 = buf;
	p2 = p - 1;
	while (p1 < p2)
	{
		uint8_t tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size)
{
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for ( size_t i = 0; i < size; i++ )
		dst[i] = src[i];
	return dstptr;
}
char* strcpy( char* arr_out, char* arr_in )
{
	char* pc;

	pc = arr_out;

	while( (*arr_out++ = *arr_in++) ) ;

	return pc;
}
