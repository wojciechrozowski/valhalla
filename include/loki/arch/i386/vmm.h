#ifndef vmm_h
#define vmm_h
#define KERNEL_VIRTUAL_BASE 0xC0000000
typedef unsigned int vaddr_t;
void init_vmm();
#endif
