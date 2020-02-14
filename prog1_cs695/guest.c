#include <stddef.h>
#include <stdint.h>
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
	int whence;// 4
	int displacement; // 4
	int num_bytes;//4
	// int isnewFile;
	char file_name[20];
	char buffer[201];// 200
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
	asm("outl %0,%1" : /* empty */ : "a" (addr), "Nd" (DISPLAY_PORT) : "memory");
}
// static char abc[5] = "HEllo world";
 
void
__attribute__((noreturn))
__attribute__((section(".start")))
_start(void) {
	const char *p;

	for (p = "Hello, world!\n"; *p; ++p)
		outb(*p);

	printVal(1234);
	printVal(4321);
	uint32_t q[20];
	q[0] = getNumExits();
	q[1] = getNumExits();
	printVal(q[0]);
	printVal(q[1]);
	q[2] = 0x500;
	const char * abc = "hi by guest\n";
	strcpy((char *)(uintptr_t)q[2],abc );
	q[3] = getNumExits();
	display(q[2]);
	q[4] = getNumExits();
	printVal(q[3]);
	printVal(q[4]);
	file_handler * T[100];
	//open demo
	// displaying error messages : open demo
	T[0] =(file_handler *) (uintptr_t) 0x10000;
	T[0]->flags = O_RDONLY; 
	strcpy(T[0]->file_name, "f0.txt");
	file_open((uint32_t)(uintptr_t)T[0]);
	display((uint32_t)(uintptr_t)&(T[0]->buffer));
	
	//display successful opening of file : open demo
	T[1] =(file_handler *) (uintptr_t) 0x10100;
	T[1]->flags = O_RDWR; 
	strcpy(T[1]->file_name, "f1.txt");
	file_open((uint32_t)(uintptr_t)T[1]);
	display((uint32_t)(uintptr_t)&(T[1]->buffer));
	T[1]->num_bytes = 10;
	file_read((uint32_t)(uintptr_t)T[1]);
	display((uint32_t)(uintptr_t)&(T[1]->buffer));
	// display 200 bytes : read demo
	T[2] =(file_handler *) (uintptr_t) 0x10200;
	T[2]->flags = O_RDWR; 
	strcpy(T[2]->file_name, "f2.txt");
	file_open((uint32_t)(uintptr_t)T[2]);
	display((uint32_t)(uintptr_t)&(T[2]->buffer));
	T[2]->num_bytes = 200;
	file_read((uint32_t)(uintptr_t)T[2]);
	display((uint32_t)(uintptr_t)&(T[2]->buffer));
	// write demo: writing 200 bytes into a file
	T[3] =(file_handler *) (uintptr_t) 0x10300;
	T[3]->flags = O_APPEND|O_RDWR;
	strcpy(T[3]->file_name, "f3.txt");
	file_open((uint32_t)(uintptr_t)T[3]);
	T[3]->num_bytes = 200;
	for(int i= 0; i<200;i++){
		T[3]->buffer[i] = T[2]->buffer[i];
	}
	file_write((uint32_t)(uintptr_t)T[3]);
	//lseek demo completed	
	T[4] =(file_handler *) (uintptr_t) 0x10400;
	T[4]->flags = O_RDWR;
	strcpy(T[4]->file_name, "f4.txt");
	file_open((uint32_t)(uintptr_t)T[4]);
	strcpy(T[4]->buffer, "agarwal\n");
	T[4]->whence = SEEK_END;
	T[4]->displacement = -1;
	T[4]->num_bytes = 8;
	file_seek((uint32_t)(uintptr_t)T[4]);
	file_write((uint32_t)(uintptr_t)T[4]);	
	
	// T[1] =(file_handler *) (uintptr_t) 0x10100;
	// T[1]->flags = O_RDWR; 
	// strcpy(T[1]->file_name, "f2.txt");
	// file_open((uint32_t)(uintptr_t)T[1]);
	// // T[2] =(file_handler *) (uintptr_t) 0x10200;
	// // T[2]->flags = O_RDWR; 
	// // strcpy(T[2]->file_name, "fileot2.txt");
	// // file_open((uint32_t)(uintptr_t)T[2]);
	// // T[3] =(file_handler *) (uintptr_t) 0x10300;
	// // T[3]->flags = O_RDWR; 
	// // strcpy(T[3]->file_name, "fileot3.txt");
	// // file_open((uint32_t)(uintptr_t)T[3]);
	// T[0]->num_bytes = 100;
	// file_read((uint32_t)(uintptr_t)T[0]);
	// display((uint32_t)(uintptr_t)&(T[0]->buffer));
	// T[1]->whence =  SEEK_CUR;
	// T[1]->displacement = 30;
	// T[1]->num_bytes = 100;
	// // file_seek((uint32_t)(uintptr_t)T[0]);
	// file_seek((uint32_t)(uintptr_t)T[1]);
	// // strcpy(T[1]->buffer, T[0]->buffer);
	// for(int i= 0; i<100;i++){
	// 	T[1]->buffer[i] = T[0]->buffer[i];
	// }

	// display((uint32_t)(uintptr_t)&(T[1]->buffer));
	// // printVal(T[1]->fd);
	// file_write((uint32_t)(uintptr_t)T[1]);

	

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
