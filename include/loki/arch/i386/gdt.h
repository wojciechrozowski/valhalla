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

#ifndef gdt_h
#define gdt_h




struct  gdt_entry
{
	uint16_t limit_low; ///< The lower 16 bytes of the limit
	uint16_t base_low; ///< The lower 16 - of the base
	uint8_t base_middle; ///< Next 8 bytes of the base
	uint8_t access; ///< Access byte
	uint8_t granularity; ///< Granularity byte
	uint8_t base_high; ///< The last 8 bytes of the base
} __attribute__ ((packed));
typedef struct gdt_entry gdt_entry_t;

struct gdt_ptr
{
	uint16_t limit; ///<The upper 16 bits of all selector limits.
	uint32_t base;
} __attribute__ ((packed));
typedef struct gdt_ptr gdt_ptr_t;

void init_gdt();

#endif
