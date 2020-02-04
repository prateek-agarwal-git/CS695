#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define DISPLAY_PORT 0x72
#define PRINTVAL_PORT 0x73
#define NUMEXITS_PORT 0x74 
#define EXAMPLE_PORT 0x75
#define READFILE_PORT 0x76
#define WRITEFILE_PORT 0x77
#define OPENFILE_PORT 0x78
#define LSEEKFILE_PORT 0x79
#define CLOSEFILE_PORT 0x80
struct demoStruct{
	int a; 
	char b[20];
};
typedef struct demoStruct demostruct;
// struct fileInfo {
// 	int fd;
// 	char fileName[16];
// 	char fileData[128];
// };
// static void fileOpen(){
// }
// static void fileRead(){
// }
// static void fileWrite(){
// }

// static void outb(uint8_t value) {
// 	asm("outb %0,%1" : /* empty */ : "a" (value), "Nd" (EXAMPLE_PORT) : "memory");
// }

// static void printVal(uint32_t value) {
// 	asm("outl %0,%1" : /* empty */ : "a" (value), "Nd" (PRINTVAL_PORT) : "memory");
// }
// static inline uint32_t getNumExits() {
// 	uint32_t ret;
// 	asm("in %1, %0" : "=a" (ret) : "Nd" (NUMEXITS_PORT) : "memory");
// 	return ret;
// }
static void display( uint32_t addr) {
	asm("outl %0,%1" : /* empty */ : "a" (addr), "Nd" (DISPLAY_PORT) : "memory");
}

 
void
__attribute__((noreturn))
__attribute__((section(".start")))
_start(void) {
	// const char *p;
	//  question !!! control switches from guest to hypervisor
	// uint32_t q = getNumExits();
	// printVal(q);
	// demostruct s;
	// s.a = 23;
	// s.b = 24
	(*(demostruct *)0x500).a= 23;
	// (*(demostruct *)0x500).b= 24;
	strcpy((*(demostruct *)0x500).b, "hello cs695\n\n\n");
	// uintptr_t t = (uintptr_t)&((*(demostruct *)0x500).b);
	// uint32_t m = (uint32_t) t;
	display((uint32_t)(uintptr_t)&((*(demostruct *)0x500).b));

	
	
	// for (p = "Balle world\n"; *p; ++p)
	// 	outb(*p);
	// uint32_t z = getNumExits();
	// printVal((*(demostruct *)0x500).a);


	// // int a;
	// // a = 1<<29;
	// // char * t = "Cye\n";
	// // *(char **)0x500 =t;
	// strcpy((char *)0x500, "CAR");
	// // *( char *)0x500 = 'C';
	// // *( char *)0x501 = 'A';
	// // *( char *)0x502 = 'R';
	// // *(int *)10000 = 789;
	// // *( char *)0x503 = '\0';
	// // printVal(789);
	// // a = sizeof(&p);
	// // printVal(0x24, a);
	// // printVal(0x24, a);
	// // printVal(0x24, a);
	// display(0x500);
	// /
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

// uint32_t q = getNumExits();
	// printVal(q);
	// q = getNumExits();
	// printVal(q);
	// printVal(q);

	// uint32_t r = getNumExits();
	// printVal(r);
	
	// uint32_t h = getNumExits();
	// printVal(h);
	// uint32_t i = getNumExits();
	// printVal(i);
	// uint32_t j = getNumExits();
	// printVal(j);
	// uint32_t k = getNumExits();
	// printVal(k);
