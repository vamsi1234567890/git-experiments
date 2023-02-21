#include <iostream>
#define __STDC_FORMAT_MACROS

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <inttypes.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <asm/unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/kernel-page-flags.h>
#include <iostream>
#include <fstream>
#include <fstream>
#include <fstream>
#include <fstream>

const size_t mem_size = 1 << 30;
using namespace std;

uint64_t getPhysicalAddress(char *g_mem, int pagemap)
{
	uint64_t page_frame_number = 0;
	uint64_t physical_address = 0;
	uint64_t value;
	int got1 = pread(pagemap, &value, 8, (reinterpret_cast<uintptr_t>((uint64_t*) g_mem) / 0x1000) * 8);
	assert(got1 == 8);
	page_frame_number = value & ((1ULL << 54)-1);
	physical_address = page_frame_number * 0x1000;
	return physical_address;
}

int getBankNumber(uint64_t curnt)	{
	int ba0 = ((curnt >> 13) & 1) ^ ((curnt >> 17) & 1);
	int ba1 = ((curnt >> 14) & 1) ^ ((curnt >> 18) & 1);
	int ba2 = ((curnt >> 16) & 1) ^ ((curnt >> 20) & 1);
	int rank = ((curnt >> 15) & 1) ^ ((curnt >> 19) & 1);
	int bank = (rank << 3) + (ba2 << 2) + (ba1 << 1) + ba0;
	return bank;
}


char *arr;

  
int main()
{

arr = (char *) mmap(NULL, mem_size, PROT_READ | PROT_WRITE,
                        MAP_ANON | MAP_PRIVATE, -1, 0);
  assert(arr != MAP_FAILED);

  ofstream MyFile("a.txt");
  memset(arr, 0xff, mem_size);
  int pagemap = open("/proc/self/pagemap", O_RDONLY);
  assert(pagemap >= 0);
  uint64_t *end = (uint64_t *) (arr + mem_size);
    uint64_t *ptr;
    
    for (ptr = (uint64_t *) arr; ptr < end; ptr=ptr+1000) {
    	uint64_t phys1 = getPhysicalAddress((char *) (ptr), pagemap);
  	int rn=((phys1 & 0x1fffc0000) >> 18);
  	int bn=getBankNumber(phys1);
  	string s= to_string(bn) + "	"+ to_string(rn)+"\n";
  	MyFile << s;
    }
    MyFile.close();
    
  
 
    return 0;
}
