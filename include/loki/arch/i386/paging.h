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
#ifndef paging_h
#define paging_h
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))


uint32_t *kernel_page_directory;
//typedef unsigned int page_table_t;
//
//typedef unsigned int page_directory_t;

void switch_page_directory(uint32_t page_directory);

void enable_paging();


void init_pm();
void map_page(uint32_t *page_directory, uint32_t phys_addr,uint32_t virt_addr,uint32_t pde_flags, uint32_t pte_flags);
void unmap_page(uint32_t* page_directory, uint32_t virt_addr);
void alloc_page(uint32_t* page_directory, uint32_t pde_flags, uint32_t pte_flags);
void alloc_pages(uint32_t* page_directory, uint32_t count, uint32_t pde_flags, uint32_t pte_flags);

void set_frame(uint32_t frame_addr);
void clear_frame(uint32_t frame_addr);
uint32_t test_frame(uint32_t frame_addr);
uint32_t first_frame();
uint32_t get_free_phys_addr();
uint32_t kmalloc_universal(uint32_t sz, int align, uint32_t *phys);
uint32_t kmalloc_a(uint32_t sz);
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys);
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys);
uint32_t kmalloc(uint32_t sz);

void* liballoc_alloc( int pages );
int liballoc_free( void* ptr, int pages );
void liballoc_free_frame(uint32_t virt_addr);

int liballoc_lock();
int liballoc_unlock();

uint32_t *frames;
uint32_t nframes;

#endif
