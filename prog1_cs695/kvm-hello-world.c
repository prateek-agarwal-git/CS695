#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <linux/kvm.h>
//  Emulation bit .if set floating point x87 bit enabled other wise not

/* CR0 bits */
#define CR0_PE 1u 
#define CR0_MP (1U << 1)
#define CR0_EM (1U << 2)
#define CR0_TS (1U << 3)
// saving x87 task context 
#define CR0_ET (1U << 4)
//  whether external math processor was 808287? or 80387?
#define CR0_NE (1U << 5)
// enables numeric error reporting
#define CR0_WP (1U << 16)
// write protect mode: CPU can't write to read only pages when privilege level is zero(kernel mode)
#define CR0_AM (1U << 18)
// Alignment check enabled and previlige level is 3
#define CR0_NW (1U << 29)
// Not write through: globally enbables/disables write through caching
#define CR0_CD (1U << 30)
// enabling/disabling cache
#define CR0_PG (1U << 31)
// if set, paging is enabled and CR 3 register is used.
// otherwise paging is disabled.
// CR3 stores page table starting location.
/* CR4 bits */
#define CR4_VME 1
//  virtual mode extensions. enables VIRTUAL INTERRUPT FLAG in vrtual 8086 mode
#define CR4_PVI (1U << 1)
//  same as above but in protected mode
#define CR4_TSD (1U << 2)
// Time stamp disable: if set, RDTCS can only be used in highest rpiority mode 
#define CR4_DE (1U << 3)
// Debugging extension: not clear 
#define CR4_PSE (1U << 4)
// Page size extension : if set , increase from 4 kb to higher size.
// if the next bit is set(Physical address extension) or the processor is in x86_64 mode, this bit is ignored.
#define CR4_PAE (1U << 5)
// Translates 32 bit virtual addresses to extended  36 bit physical addresses(whatever that means)
#define CR4_MCE (1U << 6)
// machine check exception: enables machine check INTERRUPT to occur.
#define CR4_PGE (1U << 7)
// PAGE GLOBAL ENABLE: if enabled page translation between different address spaces is enabled
#define CR4_PCE (1U << 8)
// PErformance counter enabled: if enabled, performance counter can be used in any privilege level. otherwise only in highest privilege
#define CR4_OSFXSR (1U << 8)
//  operating system support for FXSAVE(what??) and FXSTOR(what??) instructions
#define CR4_OSXMMEXCPT (1U << 10)
// enables SIMD ( single instruction multiple data) unmasked floating point exceptions 
#define CR4_UMIP (1U << 11)
// User mode instruction prevention: if current privilege level is  greater than zero, do not allow the following instructions :
// SGDT : store global descriptorTable register(??) in the destination operand
//  SIDT:store interrupt descriptor table register
//  SLDT: store local deescriptor table(??)
//  SMSW : store machine status word
//  STR  : store immediate register mode
#define CR4_VMXE (1U << 13)
//  enable virtual machine extensions( hardware assisted) : intel vt-x 86
#define CR4_SMXE (1U << 14)
// Safer mode extensions enable  : thunder trust
#define CR4_FSGSBASE (1U << 16)
// enable RDFSBASE, RDGSBASE, WRFSBASE, and WRGSBASE.
//  related to FS and GS registers
// https://stackoverflow.com/questions/10810203/what-is-the-fs-gs-register-intended-for
#define CR4_PCIDE (1U << 17)
// PRocess context identifier enable 
#define CR4_OSXSAVE (1U << 18)
#define CR4_SMEP (1U << 20)
// Supervisor mode execution enable 
#define CR4_SMAP (1U << 21)
// Supervisor mode access prevention enable : is set access of a data in a higher ring( lower privilege) causes fault

#define EFER_SCE 1
#define EFER_LME (1U << 8)
#define EFER_LMA (1U << 10)
#define EFER_NXE (1U << 11)
// above four are specific to amd. may see later
/* 32-bit page directory entry bits */
#define PDE32_PRESENT 1
#define PDE32_RW (1U << 1)
// read write flag 
#define PDE32_USER (1U << 2)
//  user/supervisor flag
#define PDE32_PS (1U << 7)
//  page size flag 
/* 64-bit page * entry bits */
#define PDE64_PRESENT 1
#define PDE64_RW (1U << 1)
#define PDE64_USER (1U << 2)
#define PDE64_ACCESSED (1U << 5)
#define PDE64_DIRTY (1U << 6)
#define PDE64_PS (1U << 7)
#define PDE64_G (1U << 8)


struct vm {
	int sys_fd;
	int fd;
	char *mem;
};
// mem_size is the total memory size alloted to vm available in memreg.memory_size
void vm_init(struct vm *vm, size_t mem_size)
{
	int api_ver;
	struct kvm_userspace_memory_region memreg;
// This structure contains: slot, flags, guest physical address, memory size, user space address.
	vm->sys_fd = open("/dev/kvm", O_RDWR);
	// 
	// vm fd as discussed in class 
	if (vm->sys_fd < 0) {
		perror("open /dev/kvm");
		exit(1);
	}
//  gets the version present on system
	api_ver = ioctl(vm->sys_fd, KVM_GET_API_VERSION, 0);
	if (api_ver < 0) {
		perror("KVM_GET_API_VERSION");
		exit(1);
	}
//  KVM api version should be 12
	if (api_ver != KVM_API_VERSION) {
		fprintf(stderr, "Got KVM api version %d, expected %d\n",
			api_ver, KVM_API_VERSION);
		exit(1);
	}
// creates a virtual machine
	vm->fd = ioctl(vm->sys_fd, KVM_CREATE_VM, 0);
	if (vm->fd < 0) {
		perror("KVM_CREATE_VM");
		exit(1);
	}
// tss : time sharing system. I do not know why it is used here
        if (ioctl(vm->fd, KVM_SET_TSS_ADDR, 0xfffbd000) < 0) {
                perror("KVM_SET_TSS_ADDR");
		exit(1);
	}
// question!!
	vm->mem = mmap(NULL, mem_size, PROT_READ | PROT_WRITE,
		   MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
	if (vm->mem == MAP_FAILED) {
		perror("mmap mem");
		exit(1);
	}

	madvise(vm->mem, mem_size, MADV_MERGEABLE);
/* 
	madvise is a system call in which we advise kernel to perform certain operations on memory.
	Following excerpt is taken from man page of madvise describing MADV_MERGEABLE flag.
	"Enable Kernel Samepage Merging (KSM) for the pages in the range specified by addr and  length.   The  kernel  regularly 
	scans those  areas  of user memory that have been marked as mergeable, looking for pages with identical content.  These 
	are replaced by a single write-protected page (which is automatically copied if a process later wants to update the 
	content of  the  page). KSM merges only private anonymous pages (see mmap(2))."*/
	memreg.slot = 0;
	memreg.flags = 0;
	memreg.guest_phys_addr = 0;
	memreg.memory_size = mem_size;
	memreg.userspace_addr = (unsigned long)vm->mem;
	/*https://lwn.net/Articles/658511/
	"The slot field provides an integer index identifying each region of memory we hand to KVM; calling 
	KVM_SET_USER_MEMORY_REGION again with the same slot will replace this mapping, while calling it with a new slot will 
	create a separate mapping. 	guest_phys_addr specifies the base "physical" address as seen from the guest, and userspace_
	addr points to the backing memory in our process that we allocated with mmap(); note that these always use 64-bit values, even on 32-bit platforms. 
	memory_size specifies how much memory to map: one page, 0x1000 bytes.
	"
	*/

    if (ioctl(vm->fd, KVM_SET_USER_MEMORY_REGION, &memreg) < 0) {
		perror("KVM_SET_USER_MEMORY_REGION");
                exit(1);
	}
}

struct vcpu {
	int fd;
	struct kvm_run *kvm_run;
};

void vcpu_init(struct vm *vm, struct vcpu *vcpu)
{
	int vcpu_mmap_size;

	vcpu->fd = ioctl(vm->fd, KVM_CREATE_VCPU, 0);
        if (vcpu->fd < 0) {
		perror("KVM_CREATE_VCPU");
                exit(1);
	}
// The KVM_RUN ioctl (cf.) communicates with userspace via a shared
// memory region.  This ioctl returns the size of that region.  See the
// KVM_RUN documentation for details.(below ioctl)
// 
//  question!! returns mmap size memory of the common region
	vcpu_mmap_size = ioctl(vm->sys_fd, KVM_GET_VCPU_MMAP_SIZE, 0);
        if (vcpu_mmap_size <= 0) {
		perror("KVM_GET_VCPU_MMAP_SIZE");
                exit(1);
	}
// question!! size = vcpu_mmap_size
// allocation in kvm_run
//  at null address of hypervisor?? Print the virtual address of kvm_run . NULL means ANY in mmap and does not mean 0.
// fd = -1 means the page may not be swapped out to disk. here this page is accessed through vcpu->fd   
	vcpu->kvm_run = mmap(NULL, vcpu_mmap_size, PROT_READ | PROT_WRITE,
			     MAP_SHARED, vcpu->fd, 0);
	if (vcpu->kvm_run == MAP_FAILED) {
		perror("mmap kvm_run");
		exit(1);
	}
}

int run_vm(struct vm *vm, struct vcpu *vcpu, size_t sz)
{
	struct kvm_regs regs;
	uint64_t memval = 0;

	for (;;) {
		if (ioctl(vcpu->fd, KVM_RUN, 0) < 0) {
			perror("KVM_RUN");
			exit(1);
		}

		switch (vcpu->kvm_run->exit_reason) {
		case KVM_EXIT_HLT:
			goto check;

		case KVM_EXIT_IO:
			if (vcpu->kvm_run->io.direction == KVM_EXIT_IO_OUT
			    && vcpu->kvm_run->io.port == 0xE9) {
				char *p = (char *)vcpu->kvm_run;
				fwrite(p + vcpu->kvm_run->io.data_offset,
				       vcpu->kvm_run->io.size, 1, stdout);
				fflush(stdout);
				continue;
			}

			/* fall through */
		default:
			fprintf(stderr,	"Got exit_reason %d,"
				" expected KVM_EXIT_HLT (%d)\n",
				vcpu->kvm_run->exit_reason, KVM_EXIT_HLT);
			exit(1);
		}
	}

 check:
	if (ioctl(vcpu->fd, KVM_GET_REGS, &regs) < 0) {
		perror("KVM_GET_REGS");
		exit(1);
	}

	if (regs.rax != 42) {
		printf("Wrong result: {E,R,}AX is %lld\n", regs.rax);
		return 0;
	}

	memcpy(&memval, &vm->mem[0x400], sz);
	if (memval != 42) {
		printf("Wrong result: memory at 0x400 is %lld\n",
		       (unsigned long long)memval);
		return 0;
	}

	return 1;
}

extern const unsigned char guest16[], guest16_end[];

int run_real_mode(struct vm *vm, struct vcpu *vcpu)
{
	struct kvm_sregs sregs;
	struct kvm_regs regs;

	printf("Testing real mode\n");

        if (ioctl(vcpu->fd, KVM_GET_SREGS, &sregs) < 0) {
		perror("KVM_GET_SREGS");
		exit(1);
	}

	sregs.cs.selector = 0;
	sregs.cs.base = 0;

        if (ioctl(vcpu->fd, KVM_SET_SREGS, &sregs) < 0) {
		perror("KVM_SET_SREGS");
		exit(1);
	}

	memset(&regs, 0, sizeof(regs));
	/* Clear all FLAGS bits, except bit 1 which is always set. */
	regs.rflags = 2;
	regs.rip = 0;

	if (ioctl(vcpu->fd, KVM_SET_REGS, &regs) < 0) {
		perror("KVM_SET_REGS");
		exit(1);
	}

	memcpy(vm->mem, guest16, guest16_end-guest16);
	return run_vm(vm, vcpu, 2);
}

static void setup_protected_mode(struct kvm_sregs *sregs)
{
	struct kvm_segment seg = {
		.base = 0,
		.limit = 0xffffffff,
		.selector = 1 << 3,
		.present = 1,
		.type = 11, /* Code: execute, read, accessed */
		.dpl = 0,
		.db = 1,
		.s = 1, /* Code/data */
		.l = 0,
		.g = 1, /* 4KB granularity */
	};

	sregs->cr0 |= CR0_PE; /* enter protected mode */

	sregs->cs = seg;

	seg.type = 3; /* Data: read/write, accessed */
	seg.selector = 2 << 3;
	sregs->ds = sregs->es = sregs->fs = sregs->gs = sregs->ss = seg;
}

extern const unsigned char guest32[], guest32_end[];

int run_protected_mode(struct vm *vm, struct vcpu *vcpu)
{
	struct kvm_sregs sregs;
	struct kvm_regs regs;

	printf("Testing protected mode\n");

        if (ioctl(vcpu->fd, KVM_GET_SREGS, &sregs) < 0) {
		perror("KVM_GET_SREGS");
		exit(1);
	}

	setup_protected_mode(&sregs);

        if (ioctl(vcpu->fd, KVM_SET_SREGS, &sregs) < 0) {
		perror("KVM_SET_SREGS");
		exit(1);
	}

	memset(&regs, 0, sizeof(regs));
	/* Clear all FLAGS bits, except bit 1 which is always set. */
	regs.rflags = 2;
	regs.rip = 0;

	if (ioctl(vcpu->fd, KVM_SET_REGS, &regs) < 0) {
		perror("KVM_SET_REGS");
		exit(1);
	}

	memcpy(vm->mem, guest32, guest32_end-guest32);
	return run_vm(vm, vcpu, 4);
}

static void setup_paged_32bit_mode(struct vm *vm, struct kvm_sregs *sregs)
{
	uint32_t pd_addr = 0x2000;
	uint32_t *pd = (void *)(vm->mem + pd_addr);

	/* A single 4MB page to cover the memory region */
	pd[0] = PDE32_PRESENT | PDE32_RW | PDE32_USER | PDE32_PS;
	/* Other PDEs are left zeroed, meaning not present. */

	sregs->cr3 = pd_addr;
	sregs->cr4 = CR4_PSE;
	sregs->cr0
		= CR0_PE | CR0_MP | CR0_ET | CR0_NE | CR0_WP | CR0_AM | CR0_PG;
	sregs->efer = 0;
}

int run_paged_32bit_mode(struct vm *vm, struct vcpu *vcpu)
{
	struct kvm_sregs sregs;
	struct kvm_regs regs;

	printf("Testing 32-bit paging\n");

        if (ioctl(vcpu->fd, KVM_GET_SREGS, &sregs) < 0) {
		perror("KVM_GET_SREGS");
		exit(1);
	}

	setup_protected_mode(&sregs);
	setup_paged_32bit_mode(vm, &sregs);

        if (ioctl(vcpu->fd, KVM_SET_SREGS, &sregs) < 0) {
		perror("KVM_SET_SREGS");
		exit(1);
	}

	memset(&regs, 0, sizeof(regs));
	/* Clear all FLAGS bits, except bit 1 which is always set. */
	regs.rflags = 2;
	regs.rip = 0;

	if (ioctl(vcpu->fd, KVM_SET_REGS, &regs) < 0) {
		perror("KVM_SET_REGS");
		exit(1);
	}

	memcpy(vm->mem, guest32, guest32_end-guest32);
	return run_vm(vm, vcpu, 4);
}
// question!!! extern arrays here . We will be only concerned 
// with 64 bit(long mode) as discussed in the question.
// Find guest code, guest page table, kernel stack
//  What is a kernel stack??

extern const unsigned char guest64[], guest64_end[];

static void setup_64bit_code_segment(struct kvm_sregs *sregs)
{
	struct kvm_segment seg = {
		.base = 0,
		.limit = 0xffffffff,
		.selector = 1 << 3,
		.present = 1,
		.type = 11, /* Code: execute, read, accessed */
		.dpl = 0,
		.db = 0,
		.s = 1, /* Code/data */
		.l = 1,
		.g = 1, /* 4KB granularity */
	};

	sregs->cs = seg;

	seg.type = 3; /* Data: read/write, accessed */
	seg.selector = 2 << 3;
	sregs->ds = sregs->es = sregs->fs = sregs->gs = sregs->ss = seg;
	// CS (code segment) DS (data segment) SS (stack segment) ES 
	// (extra segment) the 386 architecture introduced two new general segment registers FS, GS.

	// https://reverseengineering.stackexchange.com/questions/2006/how-are-the-segment-registers-fs-gs-cs-ss-ds-es-used-in-linux
}

static void setup_long_mode(struct vm *vm, struct kvm_sregs *sregs)
{
	// question!! guest page table physical address 8192 0x2000
	uint64_t pml4_addr = 0x2000;
	// question!! single page for pml4_addr table
	uint64_t *pml4 = (void *)(vm->mem + pml4_addr);
	//  question!! pml4 above is the actual virtual address in host operating system 
	// of guest page table 
	uint64_t pdpt_addr = 0x3000;
	// question!! single 4 kb page for pdp table
	// https://david942j.blogspot.com/2018/10/note-learning-kvm-implement-your-own.html

	uint64_t *pdpt = (void *)(vm->mem + pdpt_addr);
	//  question !!page directory table size 4 kb
	uint64_t pd_addr = 0x4000;
	uint64_t *pd = (void *)(vm->mem + pd_addr);
	//  three levels of page table. Each level has been allotted 4 KB page 
	pml4[0] = PDE64_PRESENT | PDE64_RW | PDE64_USER | pdpt_addr;
	pdpt[0] = PDE64_PRESENT | PDE64_RW | PDE64_USER | pd_addr;
	pd[0] = PDE64_PRESENT | PDE64_RW | PDE64_USER | PDE64_PS;

	sregs->cr3 = pml4_addr;
	// cr3 contains page table directory start. First 20 bits represent page directory table register
	sregs->cr4 = CR4_PAE;
	// physical address extension
	sregs->cr0
		= CR0_PE | CR0_MP | CR0_ET | CR0_NE | CR0_WP | CR0_AM | CR0_PG;
//  monitor co processor
// protected mode enabled+ 


	sregs->efer = EFER_LME | EFER_LMA;
	// Long mode enable + Long mode active

	setup_64bit_code_segment(sregs);
}

int run_long_mode(struct vm *vm, struct vcpu *vcpu)
{
	struct kvm_sregs sregs;
	// segment registers containg code and data
	struct kvm_regs regs;
// normal registers
	printf("Testing 64-bit mode\n");

        if (ioctl(vcpu->fd, KVM_GET_SREGS, &sregs) < 0) {
		perror("KVM_GET_SREGS");
		exit(1);
	}

	setup_long_mode(vm, &sregs);

        if (ioctl(vcpu->fd, KVM_SET_SREGS, &sregs) < 0) {
		perror("KVM_SET_SREGS");
		exit(1);
	}

	memset(&regs, 0, sizeof(regs));
	/* Clear all FLAGS bits, except bit 1 which is always set. */
	regs.rflags = 2;
	regs.rip = 0;
	/* Create stack at top of 2 MB page and grow down. */
	//  question!! kernel stack starts  at the top of the guest address space (highest address which is 2MB
	//  i.e. the size alloted to the vm) . In host virtual address space, it starts from vm-> mem + 2MB and grows 
	// downwards to lower addresses.

	regs.rsp = 2 << 20;
	if (ioctl(vcpu->fd, KVM_SET_REGS, &regs) < 0) {
		perror("KVM_SET_REGS");
		exit(1);
	}
// question!! guest code is copied from the starting address of vm->mem in the following line.
// Addresses occupied by the code section in the host virtual address space is vm-> mem to 
// vm-> mem + guest64_end


	memcpy(vm->mem, guest64, guest64_end-guest64);
	return run_vm(vm, vcpu, 8);
}


int main(int argc, char **argv)
{
	struct vm vm;
	struct vcpu vcpu;
	enum {
		REAL_MODE,
		PROTECTED_MODE,
		PAGED_32BIT_MODE,
		LONG_MODE,
	} mode = REAL_MODE;
	int opt;

	while ((opt = getopt(argc, argv, "rspl")) != -1) {
		switch (opt) {
		case 'r':
			mode = REAL_MODE;
			break;

		case 's':
			mode = PROTECTED_MODE;
			break;

		case 'p':
			mode = PAGED_32BIT_MODE;
			break;

		case 'l':
			mode = LONG_MODE;
			break;

		default:
			fprintf(stderr, "Usage: %s [ -r | -s | -p | -l ]\n",
				argv[0]);
			return 1;
		}
	}
	// vm_init(&vm, 0x200000): second argument is size 2megabyte of memory is alloted to entire virtual machine question
	vm_init(&vm, 0x200000);
	// only one vcpu in this assignment
	vcpu_init(&vm, &vcpu);

	switch (mode) {
	case REAL_MODE:
		return !run_real_mode(&vm, &vcpu);

	case PROTECTED_MODE:
		return !run_protected_mode(&vm, &vcpu);

	case PAGED_32BIT_MODE:
		return !run_paged_32bit_mode(&vm, &vcpu);

	case LONG_MODE:
		return !run_long_mode(&vm, &vcpu);
	}

	return 1;
}
// asm: machine architecture dependent instructions
// CRO control register