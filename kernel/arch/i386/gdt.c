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

extern void gdt_flush(uint32_t);

extern void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);


gdt_entry_t gdt_entries[255];
gdt_ptr_t gdt_ptr;




void init_gdt()
{

	gdt_ptr.limit = (sizeof(gdt_entry_t)*5)-1;
	gdt_ptr.base = (uint32_t)gdt_entries;

	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1,0,0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2,0,0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_gate(3,0,0xFFFFFFFF, 0xFA, 0xCF);
	gdt_set_gate(4,0,0xFFFFFFFF, 0xF2, 0xCF);


	gdt_flush((uint32_t)&gdt_ptr);
	init_msg("setting GDT");
}

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
	gdt_entries[num].base_low    = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high   = (base >> 24) & 0xFF;

	gdt_entries[num].limit_low   = (limit & 0xFFFF);
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;

	gdt_entries[num].granularity |= gran & 0xF0;
	gdt_entries[num].access      = access;
}


