#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define DISPLAY_PORT 0x72
#define PRINTVAL_PORT 0x73
#define NUMEXITS_PORT 0x74 
#define EXAMPLE_PORT 0x75
static void outb(uint8_t value) {
	asm("outb %0,%1" : /* empty */ : "a" (value), "Nd" (EXAMPLE_PORT) : "memory");
}

static void printVal(uint32_t value) {
	asm("outl %0,%1" : /* empty */ : "a" (value), "Nd" (PRINTVAL_PORT) : "memory");
}
static inline uint32_t getNumExits() {
	uint32_t ret;
	asm("in %1, %0" : "=a" (ret) : "Nd" (NUMEXITS_PORT) : "memory");
	return ret;
}
static void display( uint32_t addr) {
	// uint16_t port = 0x72;
	asm("outl %0,%1" : /* empty */ : "a" (addr), "Nd" (DISPLAY_PORT) : "memory");
}

 
void
__attribute__((noreturn))
__attribute__((section(".start")))
_start(void) {
	const char *p;
	//  question !!! control switches from guest to hypervisor
	for (p = "Balle, world!\n"; *p; ++p)
		outb(*p);
	
	// int a;
	// a = 1<<29;
	// char * t = "Cye\n";
	// *(char **)0x500 =t;
	strcpy((char *)0x500, "CAR");
	// *( char *)0x500 = 'C';
	// *( char *)0x501 = 'A';
	// *( char *)0x502 = 'R';
	// *( char *)0x503 = '\0';
	printVal(0x500);
	// a = sizeof(&p);
	// printVal(0x24, a);
	// printVal(0x24, a);
	// printVal(0x24, a);
	// uint32_t q = getNumExits();
	// printVal(0x24,q);
	display(0x500);
	*(long *) 0x400 = 42;

	for (;;)
		// control switches from guest to hypervisor
		asm("hlt" : /* empty */ : "a" (42) : "memory");
}
// static inline uint32_t inb(uint16_t port) {
//   uint8_t ret;
//   asm("in %1, %0" : "=a"(ret) : "Nd"(port) : "memory" );
//   return ret;
// }