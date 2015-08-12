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

ENTRY(_start)

SECTIONS
{
	. = 1M;
	.text BLOCK(4K) : ALIGN(4K)
	{
	
		_MULTIBOOT_START_ = .;
		*(.multiboot)
		_MULTIBOOT_END_ = .;
		_TEXT_START_ = .;
		*(.text)
		_TEXT_END_ = .;
	}

	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		_RODATA_START_ = .;
		*(.rodata)
		_RODATA_END_ = .;
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		_DATA_START_ = .;
		*(.data)
		_DATA_END_ = .;
	}

	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		_COMMON_START_ = .;
		*(COMMON)
		_COMMON_END_ = .;
		_BSS_START_ = .;
		*(.bss)
		_BSS_END_ = .;
		_BOOTSTRAP_STACK_START_ = .;
		*(.bootstrap_stack)
		_BOOTSTRAP_STACK_END_ = .;
	}

	 end = .; _end = .; __end = .;
}