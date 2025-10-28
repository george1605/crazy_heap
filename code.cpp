#include "header.h"
#include <sys/mman.h>
#include <unistd.h>
#include <cstddef>
#include <cstdint>
#include <iostream>

namespace mem
{
   size_t virt::pagesz = 0;
   void* virt::alloc(size_t pages, int prot_flags)
   {
        if(virt::pagesz == 0)
        {
            virt::pagesz = getpagesize();
        }
        void* x = mmap(NULL, pages * virt::pagesz, prot_flags, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if(x == nullptr)
        {
            throw std::bad_alloc();
        }
        return x;
   }

   void* virt::alloc_b(size_t length, int prot_flags)
   {
    if(virt::pagesz == 0)
    {
        virt::pagesz = getpagesize();
    }
    if(length % virt::pagesz != 0) // round to full page
    {
        length = length - (length % virt::pagesz) + virt::pagesz;
    }
    void* x = mmap(NULL, length, prot_flags, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(x == nullptr)
    {
        throw std::bad_alloc();
    }
    return x;
   }

   void virt::free(void* ptr, size_t pages)
   {
        munmap(ptr, virt::pagesz * pages);
   }
}

int main()
{
    void* data = mem::virt::alloc(1, PROT_READ | PROT_WRITE);
    std::cout << "Data[0]: " << ((int*)data)[0];
    mem::virt::free(data, 1);
}