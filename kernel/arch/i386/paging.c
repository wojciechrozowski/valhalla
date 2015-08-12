#include <loki/system.h>
//paging implementation
uint32_t last_kernel_page;
uint32_t PAGING_IS_ENABLED;

void init_paging()
{

	printklog("Kernel has called init_paging()!\n");


	placement_address = kernel_end;
	printklog("Calling init_pm()...\n");
	init_pm();
	printklog("Returned from init_pm().\n");

	kernel_page_directory = (uint32_t *)kmalloc_a(4096);
	printklog("Created page_directory struct located at: 0x%x.\n", (uint32_t *)kernel_page_directory);

	for(uint32_t i = 0; i < 1024; i++)
	{
		printklog("Writing not-present flags to PDE[0x%x].\n", (uint32_t) i);
	    kernel_page_directory[i] = 0x00000002;
	    printklog("PDE[0x%x] has been set to 0x%x.\n", (uint32_t) i, (uint32_t) kernel_page_directory[i]);
	}
////	uint32_t *first_page_table = (uint32_t *)kmalloc_a(4096);
//	uint32_t first_page_table[1024] __attribute__((align(4096)));
//
//	kernel_page_directory[0] = ((uint32_t)first_page_table) | 3;
//	kprintf("kernel page directory ptr: %x\n", (uint32_t *) kernel_page_directory);
//
	unsigned int address = 0;
	unsigned int i;


	for(i = 0; i < (kernel_end+8*4096)/4096; i++)
	{
		map_page(kernel_page_directory, address, address, 3, 3);
		set_frame(address);

	    address = address + 4096;
	    last_kernel_page = address;
	}

	switch_page_directory(kernel_page_directory);
	enable_paging();

	kprintf("Paging has been enabled!\n");

	switch_page_directory(kernel_page_directory);
	enable_paging();


	alloc_pages(kernel_page_directory,10,3,3);

	kprintf("Kernel space is mapped to 0x%x.\n", (uint32_t) last_kernel_page);
	printklog("Kernel space is mapped to 0x%x.\n\n", (uint32_t) last_kernel_page);
	PAGING_IS_ENABLED=1;
	uint32_t test666 =  get_free_phys_addr();
	kprintf("proba: 0x%x\n", (uint32_t) test666);

}

void switch_page_directory(uint32_t page_directory)
{
	asm volatile("mov %0, %%cr3":: "b"(page_directory));
}


void enable_paging()
{
	   uint32_t cr0;
	   asm volatile("mov %%cr0, %0": "=r"(cr0));
	   cr0 |= 0x80000000;
	   asm volatile("mov %0, %%cr0":: "r"(cr0));
}

uint32_t  get_free_phys_addr()
{
	return first_frame()*0x1000;
}
static inline void __native_flush_tlb_single(uint32_t addr)
{
   asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

void map_page(uint32_t* page_directory, uint32_t phys_addr,uint32_t virt_addr,uint32_t pde_flags, uint32_t pte_flags)
{
	uint32_t *page_table;
	uint32_t pdindex = (uint32_t)virt_addr >> 22;
	uint32_t ptindex = (uint32_t)virt_addr >> 12 & 0x03FF;

	printklog("Called map_page function\n");
	printklog("Mapping physical frame 0x%x to virtual page 0x%x\n", (uint32_t) phys_addr, (uint32_t) virt_addr);
	printklog("Adding data to page directory located at: 0x%x\n", (uint32_t *) page_directory);
	printklog("pdindex: %x\nptindex: %x\n", (uint32_t) pdindex, (uint32_t) ptindex);


	uint32_t page_directory_entry_data = page_directory[pdindex];
	printklog("page_directory[0x%x]=0x%x.\n", (uint32_t) pdindex, (uint32_t) page_directory_entry_data);


	if(!(page_directory_entry_data & 0x1))
	{
		printklog("Page table for this address' entry doesn't exist!\n");
		page_table = (uint32_t *)kmalloc_a(1024*sizeof(uint32_t));

		page_directory[pdindex] = (uint32_t ) page_table | (pde_flags & 0xFFF) | 0x01;


	}
	else
	{
		printklog("Finding pointer to page table for this page' entry.\n");
		page_table=(uint32_t *)( page_directory_entry_data & 0xFFFFF000);
		printklog("Page table is located at: 0x%x\n", (uint32_t) page_directory_entry_data & 0xFFFFF000);

	}
	page_table[ptindex] = ((uint32_t)phys_addr) | (pte_flags & 0xFFF) /*| 0x01 */;
	printklog("Added entry to page_table\n");
	printklog("PTE[0x%x]=0x%x.\n", (uint32_t) ptindex, (uint32_t)page_table[ptindex]);
	__native_flush_tlb_single(virt_addr);
	printklog("Refreshing TLB...\n");
	printklog("Done!\n\n");



}


void unmap_page(uint32_t* page_directory, uint32_t virt_addr)
{
	map_page(*page_directory, (uint32_t) 0, virt_addr, (uint32_t) 0, (uint32_t) 0);

}


void alloc_page(uint32_t* page_directory, uint32_t pde_flags, uint32_t pte_flags)
{
	printklog("Called function alloc_page().\n");
	printklog("Calling first_frame...\n");
	uint32_t free_frame = first_frame() * 0x1000;
	set_frame(first_frame() * 0x1000);
	printklog("Allocating frame loccated at 0x%x.\n", (uint32_t) free_frame);
	printklog("Calling map_page()...\n");
	map_page(page_directory, free_frame, last_kernel_page, pde_flags, pte_flags);
	last_kernel_page += 4096;
	printklog("Mapping succes.\nExiting funtion...\n");

}

void alloc_pages(uint32_t* page_directory, uint32_t count, uint32_t pde_flags, uint32_t pte_flags)
{
	for(uint32_t i=0;i<count;i++)
	{
		alloc_page(*page_directory, pde_flags, pte_flags);
	}
}

//
//physical memory manager - based on james'm tutorial - see page "6. Paging"
//

// physical memory management initialization function
void init_pm()
{
	uint32_t memory_end_page = (mem_all * 1024)-4096;
	nframes = memory_end_page / 0x1000;
	frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));

}

// Static function to set a bit in the frames bitset
void set_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
void clear_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set.
uint32_t test_frame(uint32_t frame_addr)
{
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   return (frames[idx] & (0x1 << off));
}

// Static function to find the first free frame.
uint32_t first_frame()
{
   printklog("called first_frame() function.\n");
   uint32_t i, j;
   for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
   {
	   printklog("Searching for free frame in frames[0x%x]\n", (uint32_t) i);
	   printklog("frames[0x%x] = 0x%x\n", (uint32_t) i, (uint32_t) frames[i]);
       if (frames[i] != 0xFFFFFFFF) // nothing free, exit early.
       {
           // at least one bit is free here.
           for (j = 0; j < 32; j++)
           {
        	   uint32_t toTest = 0x1 << j;

        	   printklog("Checking bit 0x%x = %x\n", (uint32_t) j, !(frames[i]&toTest));
        	   if ( !(frames[i]&toTest) )
               {
                   return i*32+j;
               }

           }
       }
       printklog("there's no free place in here. going further...\n");
   }
   return 0;
}

//
//basic memory allocating
//

uint32_t kmalloc_universal(uint32_t sz, int align, uint32_t *phys)
{
	if(PAGING_IS_ENABLED==0)
	{
			if (align == 1 && (placement_address & 0xFFFFF000))
			{
				// Align it.
				placement_address &= 0xFFFFF000;
				placement_address += 0x1000;
			}

			if (phys)
			{
				*phys = placement_address;
			}
			uint32_t tmp = placement_address;
			placement_address += sz;
			return tmp;
	}
	else
	{

		if (align == 1 && (placement_address & 0xFFFFF000))
		{
			placement_address = liballoc_alloc(1);
		}

		if (phys)
		{
				*phys = placement_address;
		}

		uint32_t tmp = placement_address;
		placement_address += sz;
		return tmp;

	}

}

uint32_t kmalloc_a(uint32_t sz) //page aligned
{
	return kmalloc_universal(sz, 1, 0);
}
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys)//return physical adress
{
	return kmalloc_universal(sz,0, phys);
}
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys) //page aligned and return physical adress
{
	return kmalloc_universal(sz, 1, phys);
}
uint32_t kmalloc(uint32_t sz) //malloc
{
	return kmalloc_universal(sz, 0, 0);
}

//liballoc hooks


int liballoc_lock()
{
	//i don't have any mutexes (yet! :-D)
	return 0;
}

int liballoc_unlock()
{
	//i don't have any mutexes (yet! :-D)
	return 0;
}

void* liballoc_alloc( int pages )
{
	printklog("Called function liballoc_alloc()\n");
	alloc_pages(kernel_page_directory, pages, 3, 3);
	void *return_ptr = (void*) last_kernel_page;
	return return_ptr;

}

int liballoc_free( void* ptr, int pages )
{

//
//
	printklog("Called function liballoc_free().\n");
	uint32_t virt_addr = (uint32_t) ptr;
	for (int i = 0; i < pages; i++)
	{
		printklog("Freeing 0x%x.\n", virt_addr);
		liballoc_free_frame(virt_addr);
		virt_addr +=0x1000;
	}
	printklog("Returning from liballoc_free() function...");


	//return munmap( ptr, pages * page_size );
}

void liballoc_free_frame(uint32_t virt_addr)
{
	printklog("Called liballoc_free_frame() function.\n");
	uint32_t *page_table;
	printklog("We are going to relase memory starting from 0x%x.\n", virt_addr);
	uint32_t pdindex = (uint32_t)virt_addr >> 22;
	uint32_t ptindex = (uint32_t)virt_addr >> 12 & 0x03FF;
	uint32_t page_directory_entry_data = kernel_page_directory[pdindex];
	printklog("page_directory[0x%x]=0x%x.\n", (uint32_t) pdindex, (uint32_t) page_directory_entry_data);
	printklog("Finding pointer to page table for this page' entry.\n");
	page_table=(uint32_t *)( page_directory_entry_data & 0xFFFFF000);
	printklog("Page table is located at: 0x%x\n", (uint32_t) page_directory_entry_data & 0xFFFFF000);

	uint32_t frame_addr =  page_table[ptindex] & 0xFFFFF000;
	printklog("Phys frame address is 0x%x.\n", (uint32_t) frame_addr);
	printklog("Setting phys frame as free...\n");
	clear_frame(frame_addr);
	printklog("Done.\n");
	printklog("Calling unmap...\n");
	unmap_page(kernel_page_directory, virt_addr);
	printklog("Returned from unmap().\n");
	printklog("Returning from liballoc_free_frame() function.\n");
}

//void get_mapping()
//{
//	printklog("Called get_mapping() function.\n");
//	uint32_t *page_table;
//	printklog("We are going to get phys addr of 0x%x page\n", virt_addr);
//	uint32_t pdindex = (uint32_t)virt_addr >> 22;
//	uint32_t ptindex = (uint32_t)virt_addr >> 12 & 0x03FF;
//	uint32_t page_directory_entry_data = kernel_page_directory[pdindex];
//	printklog("page_directory[0x%x]=0x%x.\n", (uint32_t) pdindex, (uint32_t) page_directory_entry_data);
//	printklog("Finding pointer to page table for this page' entry.\n");
//	page_table=(uint32_t *)( page_directory_entry_data & 0xFFFFF000);
//	printklog("Page table is located at: 0x%x\n", (uint32_t) page_directory_entry_data & 0xFFFFF000);
//
//	uint32_t frame_addr =  page_table[ptindex] & 0xFFFFF000;
//	printklog("Phys frame address is 0x%x.\n", (uint32_t) frame_addr);
//}

