#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
struct file_struct{
	int fd; // 4
	int flags; // 4
	int retval;// 4
	int displacement; // 4
	int num_bytes;
	char file_name[20]; // 16 maximum size of the filename string is 16
	char buffer[200];// 200
	
	// total size of the structure is kept 24 bytes less than required
};
typedef struct file_struct file_handler;

static void file_open(uint32_t addr){
	asm("outl %0,%1" : /* empty */ : "a" (addr), "Nd" (OPENFILE_PORT) : "memory");
}
static void file_read(uint32_t addr){
	asm("outl %0,%1" : /* empty */ : "a" (addr), "Nd" (READFILE_PORT) : "memory");
}
static void file_write(uint32_t addr){
	asm("outl %0,%1" : /* empty */ : "a" (addr), "Nd" (WRITEFILE_PORT) : "memory");
}
static void file_seek(uint32_t addr){
	asm("outl %0,%1" : /* empty */ : "a" (addr), "Nd" (LSEEKFILE_PORT) : "memory");
	
}
// static void file_seek(){

// }
// 

// static void outb(uint8_t value) {
// 	asm("outb %0,%1" : /* empty */ : "a" (value), "Nd" (EXAMPLE_PORT) : "memory");
// }

static void printVal(uint32_t value) {
	asm("outl %0,%1" : /* empty */ : "a" (value), "Nd" (PRINTVAL_PORT) : "memory");
}
static inline uint32_t getNumExits() {
	uint32_t ret;
	asm("in %1, %0" : "=a" (ret) : "Nd" (NUMEXITS_PORT) : "memory");
	return ret;
}
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
	// (*(demostruct *)0x500).a= 23;
	// // (*(demostruct *)0x500).b= 24;
	// strcpy((*(demostruct *)0x500).b, "hello cs695\n\n\n");
	// uintptr_t t = (uintptr_t)&((*(demostruct *)0x500).b);
	// uint32_t m = (uint32_t) t;
	// display((uint32_t)(uintptr_t)&((*(demostruct *)0x500).b));
	file_handler * T[10];
	T[0] =(file_handler *) (uintptr_t) 0x10000;
	// uint32_t q = getNumExits();
	// uint32_t r = getNumExits();
	// uint32_t s = getNumExits();
	// uint32_t t = getNumExits();
	// uint32_t u = getNumExits();

	T[0]->flags = O_RDWR; 
	strcpy(T[0]->file_name, "fileot.txt");
	// file_open(0x10000);
	file_open((uint32_t)(uintptr_t)T[0]);
	// printVal(1);
	T[0]->num_bytes = 10;
	file_read((uint32_t)(uintptr_t)T[0]);
	display((uint32_t)(uintptr_t)&(T[0]->buffer));

	T[0]->displacement = 1;
	file_seek((uint32_t)(uintptr_t)T[0]);
	file_write((uint32_t)(uintptr_t)T[0]);

	// printVal(T[0]->fd);

	uint32_t g = getNumExits();
	printVal(g);
	// T[1] =(file_handler *) (uintptr_t) 0x11000;
	// // uint32_t q = getNumExits();
	// // uint32_t r = getNumExits();
	// // uint32_t s = getNumExits();
	// // uint32_t t = getNumExits();
	// // uint32_t u = getNumExits();

	// T[1]->flags = O_RDWR; 
	// strcpy(T[1]->file_name, "fileot2.txt");
	// // // file_open(0x10000);
	// file_open((uint32_t)(uintptr_t)T[1]);
	// // // printVal(1);
	// display((uint32_t)(uintptr_t)&(T[1]->buffer));
	// printVal(T[1]->fd);
	// g = getNumExits();
	// printVal(g);
	// printVal(q);
	// printVal(r);
	// printVal(s);
	// printVal(t);
	// printVal(u);

	

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
