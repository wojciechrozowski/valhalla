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

#ifndef idt_h
#define idt_h

struct idt_entry
{
   uint16_t base_lo;
   uint16_t sel;
   uint8_t  always0;
   uint8_t  flags;
   uint16_t base_hi;
} __attribute__((packed));
typedef struct idt_entry idt_entry_t;


struct idt_ptr
{
   uint16_t limit;
   uint32_t base;
} __attribute__((packed));
typedef struct idt_ptr idt_ptr_t;

struct regs
{

 uint32_t gs; //segment registers
 uint32_t fs;
 uint32_t es;
 uint32_t ds;

 uint32_t edi; //pushal
 uint32_t esi;
 uint32_t ebp;
 uint32_t old_esp;
 uint32_t ebx;
 uint32_t edx;
 uint32_t ecx;
 uint32_t eax;

 uint32_t intr_no; //pushed by irq/isr(n)
 uint32_t err_code;


 uint32_t eip; //pushed by CPU
 uint32_t cs;
 uint32_t eflags;


 uint32_t esp;
 uint32_t ss;
 uint32_t v86_es; //for v86_mode: TODO
 uint32_t v86_ds;
 uint32_t v86_fs;
 uint32_t v86_gs;
} __attribute__((packed));

void init_idt();
extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void	irq0();
extern void	irq1();

void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_remap();
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
#endif
