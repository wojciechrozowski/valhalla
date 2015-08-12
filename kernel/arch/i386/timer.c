#include <loki/system.h>

unsigned int timer_ticks = 0;

void timer_handler(struct regs *r)
{
	kprintf("TICK %u\n", (unsigned) timer_ticks);
	timer_ticks++;

}

void init_timer(uint32_t frequency)
{

	irq_install_handler(0, timer_handler);


	uint32_t divisor = 1193180 / frequency;


	outb(0x43, 0x36);


	uint8_t l = (uint8_t)(divisor & 0xFF);
	uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );


	outb(0x40, l);
	outb(0x40, h);
}
