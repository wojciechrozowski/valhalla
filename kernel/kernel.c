/*    Loki 0.01 operating system
 *    Copyright (C) 2014 Wojciech Ró¿owski
 *
 *    This program is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
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

uint32_t elf_sec_end;

extern uint32_t _MULTIBOOT_START_;
uint32_t multiboot_start = (uint32_t)&_MULTIBOOT_START_;

extern uint32_t _MULTIBOOT_END_;
uint32_t multiboot_end = (uint32_t)&_MULTIBOOT_END_;

extern uint32_t _TEXT_START_;
uint32_t text_start = (uint32_t)&_TEXT_START_;

extern uint32_t _TEXT_END_;
uint32_t text_end = (uint32_t)&_TEXT_END_;

extern uint32_t _RODATA_START_;
uint32_t rodata_start = (uint32_t)&_RODATA_START_;

extern uint32_t _RODATA_END_;
uint32_t rodata_end = (uint32_t)&_RODATA_END_;

extern uint32_t _DATA_START_;
uint32_t data_start = (uint32_t)&_DATA_START_;

extern uint32_t _DATA_END_;
uint32_t data_end = (uint32_t)&_DATA_END_;

extern uint32_t _COMMON_START_;
uint32_t common_start = (uint32_t)&_COMMON_START_;

extern uint32_t _COMMON_END_;
uint32_t common_end = (uint32_t)&_COMMON_END_;

extern uint32_t _BSS_START_;
uint32_t bss_start = (uint32_t)&_BSS_START_;

extern uint32_t _BSS_END_;
uint32_t bss_end = (uint32_t)&_BSS_END_;

extern uint32_t _BOOTSTRAP_STACK_START_;
uint32_t bootstrap_stack_start = (uint32_t)&_BOOTSTRAP_STACK_START_;

extern uint32_t _BOOTSTRAP_STACK_END_;
uint32_t bootstrap_stack_end = (uint32_t)&_BOOTSTRAP_STACK_END_;

void kernel_main(unsigned long magic, unsigned long addr)
{

	multiboot_info_t *mbi;
	terminal_initialize();
	init_debug();
	if (magic != 0x2BADB002)
	{
		panic("booted by non multiboot loader!");
	}
	mbi = (multiboot_info_t *)addr;

	if (CHECK_FLAG (mbi->flags, 0))
	{
		lower = mbi->mem_lower;
		upper = mbi->mem_upper;
	}

	if (CHECK_FLAG (mbi->flags, 1))
		printklog ("[mboot diag] boot_device = 0x%x\n", (unsigned) mbi->boot_device);


	if (CHECK_FLAG (mbi->flags, 2))
		printklog ("[mboot diag] cmdline = %s\n", (char *) mbi->cmdline);


	if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5))
	{
		panic("[mboot diag] Both bits 4 and 5 are set.\n");
		return;
	}
	printklog("\n");
	if (CHECK_FLAG (mbi->flags, 0))
	{
		mem_all = mbi->mem_lower + mbi->mem_upper + 512 + 1;
		printklog ("%u KB of memory detected\n", (unsigned) mem_all);
	}
	kprintf("start:\t addr: 0x100000\n");
	kprintf("head:\tstart: 0x%x\tend: 0x%x\n", (uint32_t) multiboot_start, (uint32_t) multiboot_end);
	kprintf("text:\tstart: 0x%x\tend: 0x%x\n", (uint32_t) text_start, (uint32_t) text_end);
	kprintf("rodata:\tstart: 0x%x\tend: 0x%x\n", (uint32_t) rodata_start, (uint32_t) rodata_end);
	kprintf("data:\tstart: 0x%x\tend: 0x%x\n", (uint32_t) data_start, (uint32_t) data_end);
	kprintf("common:\tstart: 0x%x\tend: 0x%x\n", (uint32_t) common_start, (uint32_t) common_end);
	kprintf("bss:\tstart: 0x%x\tend: 0x%x\n", (uint32_t) bss_start, (uint32_t) bss_end);
	kprintf("stack:\tstart: 0x%x\tend: 0x%x\n", (uint32_t) bootstrap_stack_start, (uint32_t)bootstrap_stack_end);

	if (CHECK_FLAG (mbi->flags, 5))
	{
		multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);
		kprintf("elf:\tstart: 0x%x\tend: 0x%x\n", (uint32_t) multiboot_elf_sec->addr, (uint32_t) (multiboot_elf_sec->addr + multiboot_elf_sec->size));
		elf_sec_end = multiboot_elf_sec->addr + multiboot_elf_sec->size;
	}

	if (CHECK_FLAG (mbi->flags, 3))
	{
		multiboot_module_t *mod;
		mod = (multiboot_module_t *) mbi->mods_addr;
		for(uint32_t i=0;i<mbi->mods_count;i++)
		{
			kprintf ("m 0x%x:\tstart: 0x%x\tend: 0x%x\n",(uint32_t) (i+1), (uint32_t) mod->mod_start,(uint32_t) mod->mod_end);
			modules[i].start = (uint32_t) mod->mod_start;
			modules[i].end = (uint32_t) mod->mod_end;
			mod++;
		}
	}
	printklog("\n");
	if(mbi->mods_count>0)
	{
		kprintf("end: \t addr: 0x%x\n", (uint32_t) modules[(mbi->mods_count)-1].end);
		kernel_end = modules[(mbi->mods_count)-1].end + 0x1000;

	}
	else
	{
		kprintf("end: \t addr: 0x%x\n", (uint32_t) elf_sec_end);

		kernel_end = elf_sec_end + 0x1000;
	}

	kernel_end = kernel_end - (kernel_end%0x1000);
	kprintf("kend: \t addr: 0x%x\n", (uint32_t) kernel_end);

	init_gdt();
	irq_remap();
	init_idt();
	init_vmm();


	asm ("cli");
	asm ("sti");





}
