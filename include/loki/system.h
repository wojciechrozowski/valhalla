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

#ifndef system_h
#define system_h
#include <stdint.h>
#include <stddef.h>




#include <loki/kprintf.h>
#include <loki/panic.h>
#include <loki/string.h>
#include <loki/multiboot.h>

//#include <liballoc.h>

#ifdef i386

#include <loki/arch/i386/gdt.h>
#include <loki/arch/i386/idt.h>
#include <loki/arch/i386/io.h>
#include <loki/arch/i386/screen.h>
#include <loki/arch/i386/serial_debug.h>
#include <loki/arch/i386/timer.h>

//#include <loki/arch/i386/thread.h>



uint32_t kernel_end, placement_address;

typedef struct module
{
	uint32_t start;
	uint32_t end;
}module_t;
module_t modules[255];

uint32_t lower, upper, mem_all;

#endif

#ifdef arm
//#include <loki/arch/arm/watchdog.h>
#endif


#endif
