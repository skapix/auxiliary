#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

const char * g_sm_name = "/test.shm";
const size_t g_sm_size = 1*1024*1024;

#define d_pe_exit(descr, funcname)\
	if ((int)(descr) == -1) {\
		printf("funcname: %s\n", strerror(errno));\
		return 0;\
	}

int main() {
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	int descr = shm_open(g_sm_name, O_RDWR | O_CREAT, mode); 
	d_pe_exit(descr, shm_open);
	d_pe_exit(ftruncate(descr, g_sm_size), ftruncate);
	void * mem = mmap(NULL, g_sm_size, PROT_READ | PROT_WRITE, MAP_SHARED, descr, 0);
	d_pe_exit(mem, mmap);
	//some work
	memset(mem, 13, g_sm_size);
	int c = getchar();
	munmap(mem, g_sm_size);
	shm_unlink(g_sm_name); 

}
